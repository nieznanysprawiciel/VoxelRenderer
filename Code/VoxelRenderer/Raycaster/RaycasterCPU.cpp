#include "RaycasterCPU.h"

#include "swCommonLib/Common/Multithreading/ThreadsLatch.h"


#include <iostream>


namespace vr
{




const StepDirection StepXPlus = 0;
const StepDirection StepYPlus = 1;
const StepDirection StepZPlus = 2;

const StepDirection StepXMinus = 3;
const StepDirection StepYMinus = 4;
const StepDirection StepZMinus = 5;

const uint8 PositiveOUT = 8;
const uint8 NegativeOUT = 9;


// ================================ //
//
uint32			FloatAsInt		( float number )
{
	return reinterpret_cast< uint32& >( number );
}

// ================================ //
//
float			IntAsFloat		( int number )
{
	return reinterpret_cast< float& >( number );
}


/**@brief Transition table for node children.

OctreeNode contains child mask. Every child in octree has it's position in this mask.
When we traverse from one child to another along one axis, this table gives us next @ref ChildFlag on ray path.
Note that sometimes we cross node's boundary and we are outside of node.

First index is ChildFlag of node before step. Second index is transition axis { x, y, z, -x, -y, -z }.

Remember that these values are bit shifts to child position in masks not mask itself.*/
ChildFlag StepTable[ 8 ][ 6 ] =
{
	{ 1, PositiveOUT, PositiveOUT, NegativeOUT, 4, 2 },
	{ PositiveOUT, PositiveOUT, PositiveOUT, 0, 5, 3 },
	{ 3, PositiveOUT, 0, NegativeOUT, 6, NegativeOUT },
	{ PositiveOUT, PositiveOUT, 1, 2, 7, NegativeOUT },
	
	{ 5, 0, PositiveOUT, NegativeOUT, NegativeOUT, 6 },
	{ PositiveOUT, 1, PositiveOUT, 4, NegativeOUT, 7 },
	{ 7, 2, 4, NegativeOUT, NegativeOUT, NegativeOUT },
	{ PositiveOUT, 3, 5, 6, NegativeOUT, NegativeOUT },
};


using namespace DirectX;

// ================================ //
//
RaycasterCPU::RaycasterCPU()
	:	m_height( 0 )
	,	m_width( 0 )
	,	m_renderer( nullptr )
	,	m_resourceManager( nullptr )
	,	m_end( false )
	,	m_raycastEndBarrier( std::thread::hardware_concurrency() )
	,	m_raycastLoopBarrier( std::thread::hardware_concurrency() )
{}

// ================================ //
//
RaycasterCPU::~RaycasterCPU()
{
	m_end = true;

	if( m_threadPool.size() != 0 )
	{
		m_raycastLoopBarrier.ArriveAndWait();

		for( auto& thread : m_threadPool )
			thread.join();
	}
}

// ================================ //
//
void			RaycasterCPU::Init			( IRenderer* renderer, ResourceManager* resourceManager )
{
	m_resourceManager = resourceManager;

	PrepareThreads();
}


// ================================ //
//
void			RaycasterCPU::Render		( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )
{
	if( m_width != camera->GetWidth() || m_height != camera->GetHeight() )
		ReallocateRenderBuffer( (uint16)camera->GetWidth(), (uint16)camera->GetHeight() );

	SpawnThreads( octree, camera );
	UpdateRenderTarget( m_renderBuffer.get(), svoRenderTarget );
}

// ================================ //
//
void			RaycasterCPU::SpawnThreads				( OctreePtr octree, CameraActor* camera )
{
	// Prepare data for threads
	for( auto& threadData : m_threadData )
	{
		threadData.Octree = octree;
		threadData.Camera = camera;
	}

	// Spawn all threads.
	if( m_threadPool.size() == 0 )
	{
		// First loop. Start threads
		for( unsigned int i = 1; i < GetNumThreads(); ++i )
		{
			m_threadPool.push_back( std::thread( &RaycasterCPU::RaycasterThread, this, i ) );
		}
	}
	else
	{
		// Start raycasting - notify threads.
		m_raycastLoopBarrier.ArriveAndWait();
	}

	// GUI threads works on his part of buffer too.
	RaycasterThreadImpl( m_threadData[ 0 ], 0 );
}

// ================================ //
//
void			RaycasterCPU::RaycasterThread			( Size threadNumber )
{
	while( !m_end )
	{
		RaycasterThreadImpl( m_threadData[ threadNumber ], threadNumber );

		// Block until next frame.
		m_raycastLoopBarrier.ArriveAndWait();
	}
}

// ================================ //
//
void			RaycasterCPU::PrepareThreads()
{
	auto numThreads = GetNumThreads();
	assert( numThreads );
	m_threadPool.reserve( numThreads - 1 );		// Note: GUI thread will work too.
	m_threadData.resize( numThreads );
}

//====================================================================================//
//			Raycaster core	
//====================================================================================//

// ================================ //
//
// Based on: https://research.nvidia.com/publication/efficient-sparse-voxel-octrees
void			RaycasterCPU::RaycasterThreadImpl		( ThreadData& data, Size threadNumber )
{
	std::vector< uint32 > colors =
	{ 
		0xFF0000FF,
		0xFF32CD32,
		0xFFBC8F8F,
		0xFFFFEBCD,
		0xFFFAA460,
		0xFFB0C4DE,
		0xFFFF8C00,
		0xFF778899
	};

	// For all pixels in range for this thread.
	for( uint32 pix = data.StartRange; pix < data.EndRange; ++pix )
	{
		RaycasterContext rayCtx;
		rayCtx.Octree = data.Octree;
		rayCtx.NodesStack.resize( rayCtx.Octree->GetMaxDepth() + 1 );
		
		// Find starting position
		DirectX::XMFLOAT3 direction = ComputeRayDirection( data.Camera, pix % m_width, pix / m_width );
		DirectX::XMFLOAT3 position = ComputeRayPosition( data.Camera, pix % m_width, pix / m_width );
		
		InitRaycasting( position, direction, rayCtx );
		
		const OctreeNode* childDescriptor = nullptr;


		// Write proper condition.
		while( true )
		{
			if( !childDescriptor )
				childDescriptor = &rayCtx.Octree->GetNode( rayCtx.Current );

			// Terminate.
			if( IsLeaf( childDescriptor ) )
				break;

			XMFLOAT3 corner = ParamLine( rayCtx.Position, rayCtx );
			float tc_max = Min( corner );

			ChildFlag childShift = rayCtx.ChildIdx ^ rayCtx.OctantMask; // permute child slots based on the mirroring

			if( ExistsChild( childDescriptor, childShift ) && rayCtx.tMin <= rayCtx.tMax )
			{
				//// Terminate if the voxel is small enough.
				//if (tc_max * ray.dir_sz + ray_orig_sz >= scale_exp2)
				//	break; // at t_min

				float tv_max = fminf( rayCtx.tMax, tc_max );
				float half = rayCtx.ScaleExp * 0.5f;				// Half of current node cube dimmension.

				// This line computes intersection with 
				XMFLOAT3 tCenter = ParamLine( XMFLOAT3( half, half, half ), rayCtx.tCoeff, corner );

				if( rayCtx.tMin <= tv_max )
				{
					//// Terminate if the corresponding bit in the non-leaf mask is not set.
					//if( ( child_masks & 0x0080 ) == 0 )
					//	break; // at t_min (overridden with tv_min).

					if( tc_max < rayCtx.h )
						PushOnStack( rayCtx, rayCtx.Scale, rayCtx.Current, rayCtx.tMax );

					rayCtx.h = tc_max;

					// Find child descriptor corresponding to the current voxel.
					uint32 childOffset = 0;
					if( IsIndirectPointer( childDescriptor ) ) // far
					{
						childOffset = GetIndirectPtr( rayCtx, childDescriptor );
					}
					else
					{
						childOffset = rayCtx.Current + childDescriptor->ChildPackPtr;
					}

					childOffset += CountNodesBefore( childShift, childDescriptor->ChildMask );
					rayCtx.Current = childOffset;


					// Select child voxel that the ray enters first.
					rayCtx.ChildIdx = 0;
					rayCtx.Scale--;
					rayCtx.ScaleExp = half;

					if( tCenter.x > rayCtx.tMin ) rayCtx.ChildIdx ^= 1, rayCtx.Position.x += rayCtx.ScaleExp;
					if( tCenter.y > rayCtx.tMin ) rayCtx.ChildIdx ^= 2, rayCtx.Position.y += rayCtx.ScaleExp;
					if( tCenter.z > rayCtx.tMin ) rayCtx.ChildIdx ^= 4, rayCtx.Position.z += rayCtx.ScaleExp;

					// Update active t-span and invalidate cached child descriptor.

					rayCtx.tMax = tv_max;
					childDescriptor = nullptr;
					continue;

				}
			}

			// ADVANCE
			// Step along the ray.

			int step_mask = 0;
			if( corner.x <= tc_max ) step_mask ^= 1, rayCtx.Position.x -= rayCtx.ScaleExp;
			if( corner.y <= tc_max ) step_mask ^= 2, rayCtx.Position.y -= rayCtx.ScaleExp;
			if( corner.z <= tc_max ) step_mask ^= 4, rayCtx.Position.z -= rayCtx.ScaleExp;

			// Update active t-span and flip bits of the child slot index.

			rayCtx.tMin = tc_max;
			rayCtx.ChildIdx ^= step_mask;

			// Proceed with pop if the bit flips disagree with the ray direction.

			if( ( rayCtx.ChildIdx & step_mask ) != 0 )
			{
				// POP
				// Find the highest differing bit between the two positions.

				unsigned int differing_bits = 0;
				if( ( step_mask & 1 ) != 0 ) differing_bits |= FloatAsInt( rayCtx.Position.x ) ^ FloatAsInt( rayCtx.Position.x + rayCtx.ScaleExp );
				if( ( step_mask & 2 ) != 0 ) differing_bits |= FloatAsInt( rayCtx.Position.y ) ^ FloatAsInt( rayCtx.Position.y + rayCtx.ScaleExp );
				if( ( step_mask & 4 ) != 0 ) differing_bits |= FloatAsInt( rayCtx.Position.z ) ^ FloatAsInt( rayCtx.Position.z + rayCtx.ScaleExp );
				rayCtx.Scale = ( FloatAsInt( (float)differing_bits ) >> 23 ) - 127; // position of the highest bit
				rayCtx.ScaleExp = IntAsFloat( ( rayCtx.Scale - rayCtx.Octree->GetMaxDepth() + 127 ) << 23 ); // exp2f(scale - s_max)

				// Restore parent voxel from the stack.

				auto stackElement = ReadStack( rayCtx, rayCtx.Scale );
				rayCtx.Current = stackElement.Node;
				rayCtx.tMax = stackElement.tMax;
				

				// Round cube position and extract child slot index.

				int shx = FloatAsInt( rayCtx.Position.x ) >> rayCtx.Scale;
				int shy = FloatAsInt( rayCtx.Position.y ) >> rayCtx.Scale;
				int shz = FloatAsInt( rayCtx.Position.z ) >> rayCtx.Scale;
				rayCtx.Position.x = IntAsFloat( shx << rayCtx.Scale );
				rayCtx.Position.y = IntAsFloat( shy << rayCtx.Scale );
				rayCtx.Position.z = IntAsFloat( shz << rayCtx.Scale );
				rayCtx.ChildIdx  = ( shx & 1 ) | ( ( shy & 1 ) << 1 ) | ( ( shz & 1 ) << 2 );

				// Prevent same parent from being stored again and invalidate cached child descriptor.

				rayCtx.h = 0.0f;
				childDescriptor = nullptr;
			}

		}


		// Shading
		OctreeLeaf leaf = GetResultLeafNode( rayCtx );
		const VoxelAttributes& attributes = GetLeafAttributes( leaf, rayCtx );

		// Fill pixel
		//data.Buffer[ pix ] = colors[ threadNumber ];
		data.Buffer[ pix ] = attributes.Color.c;
	}

	// All threads must end before buffer will be furthr processed.
	m_raycastEndBarrier.ArriveAndWait();
}

// ================================ //
//
uint16			RaycasterCPU::GetNumThreads() const
{
#ifdef _DEBUG
	return 1;
#else
	return (uint16)std::thread::hardware_concurrency();
#endif // DEBUG
}

// ================================ //
//
DirectX::XMFLOAT3		RaycasterCPU::ComputeRayPosition		( CameraActor* camera, int screenX, int screenY )
{
	/// @todo perspective camera
	auto cameraData = camera->GetCameraData();
	
	float aspect = cameraData.Width / cameraData.Height;

	float xFactor = screenX / ( cameraData.Width / 2 ) - 1.0f;
	float yFactor = 1.0f - screenY / ( cameraData.Height / 2 );

	XMVECTOR position = cameraData.GetPosition();
	position = XMVectorScale( cameraData.GetUpVector(), yFactor * cameraData.ViewportSize ) + position;
	position = XMVectorScale( cameraData.GetRightVector(), xFactor * aspect * cameraData.ViewportSize ) + position;

	XMFLOAT3 resultPos;
	XMStoreFloat3( &resultPos, position );
	return resultPos;
}

// ================================ //
//
DirectX::XMFLOAT3		RaycasterCPU::ComputeRayDirection		( CameraActor* camera, int screenX, int screenY )
{
	/// @todo perspective camera

	auto cameraData = camera->GetCameraData();
	return cameraData.Direction;
}

// ================================ //
//
const OctreeNode&		RaycasterCPU::FindStartingNode			( const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& direction, RaycasterContext& raycasterContext )
{
	assert( false );

	return raycasterContext.Octree->AccessOctree()[ 0 ];
}

// ================================ //
//
void					RaycasterCPU::InitRaycasting			( const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& direction, RaycasterContext& rayCtx )
{
	const float epsilon = exp2f( -(float)rayCtx.Octree->GetMaxDepth() );

	rayCtx.RayStartPosition = position;
	rayCtx.RayDirection = direction;

    // Get rid of small ray direction components to avoid division by zero.

	if( fabsf( rayCtx.RayDirection.x ) < epsilon ) rayCtx.RayDirection.x = copysignf( epsilon, rayCtx.RayDirection.x );
	if( fabsf( rayCtx.RayDirection.y ) < epsilon ) rayCtx.RayDirection.y = copysignf( epsilon, rayCtx.RayDirection.y );
	if( fabsf( rayCtx.RayDirection.z ) < epsilon ) rayCtx.RayDirection.z = copysignf( epsilon, rayCtx.RayDirection.z );

	// Precompute the coefficients of tx(x), ty(y), and tz(z).
	// The octree is assumed to reside at coordinates [1, 2].

	rayCtx.tCoeff.x = 1.0f / -fabs( rayCtx.RayDirection.x );
	rayCtx.tCoeff.y = 1.0f / -fabs( rayCtx.RayDirection.y );
	rayCtx.tCoeff.z = 1.0f / -fabs( rayCtx.RayDirection.z );

	rayCtx.tBias.x = rayCtx.tCoeff.x * rayCtx.RayStartPosition.x;
	rayCtx.tBias.y = rayCtx.tCoeff.y * rayCtx.RayStartPosition.y;
	rayCtx.tBias.z = rayCtx.tCoeff.z * rayCtx.RayStartPosition.z;

	// Choose correct child flag.
    // Select octant mask to mirror the coordinate system so
    // that ray direction is negative along each axis.

	rayCtx.OctantMask = 7;
	if( rayCtx.RayDirection.x > 0.0f ) rayCtx.OctantMask ^= 1, rayCtx.tBias.x = ParamLineX( 3.0f, rayCtx );
	if( rayCtx.RayDirection.y > 0.0f ) rayCtx.OctantMask ^= 2, rayCtx.tBias.y = ParamLineY( 3.0f, rayCtx );
	if( rayCtx.RayDirection.z > 0.0f ) rayCtx.OctantMask ^= 4, rayCtx.tBias.z = ParamLineZ( 3.0f, rayCtx );


	// Initialize the active span of t-values.
	rayCtx.tMin = fmaxf( fmaxf( ParamLineX( 2.0f, rayCtx ), ParamLineY( 2.0f, rayCtx ) ), ParamLineZ( 2.0f, rayCtx ) );
	rayCtx.tMax = fminf( fminf( ParamLineX( 1.0f, rayCtx ), ParamLineY( 1.0f, rayCtx ) ), ParamLineZ( 1.0f, rayCtx ) );
	rayCtx.h = rayCtx.tMax;
	rayCtx.tMin = fmaxf( rayCtx.tMin, 0.0f );
	rayCtx.tMax = fminf( rayCtx.tMax, 1.0f );

	rayCtx.Current = rayCtx.Octree->GetRootNodeOffset();
	rayCtx.Scale = rayCtx.Octree->GetMaxDepth() - 1;
	rayCtx.ScaleExp = 0.5f;
	rayCtx.ChildIdx = 0;
	rayCtx.Position = XMFLOAT3( 1.0f, 1.0f, 1.0f );

	if( 1.5f * rayCtx.tCoeff.x - rayCtx.tBias.x > rayCtx.tMin ) rayCtx.ChildIdx ^= 1, rayCtx.Position.x = 1.5f;
	if( 1.5f * rayCtx.tCoeff.y - rayCtx.tBias.y > rayCtx.tMin ) rayCtx.ChildIdx ^= 2, rayCtx.Position.y = 1.5f;
	if( 1.5f * rayCtx.tCoeff.z - rayCtx.tBias.z > rayCtx.tMin ) rayCtx.ChildIdx ^= 4, rayCtx.Position.z = 1.5f;
}


// ================================ //
//
const OctreeLeaf&		RaycasterCPU::GetResultLeafNode		( RaycasterContext& raycasterContext ) const
{
	const OctreeNode& node = raycasterContext.Octree->GetNode( raycasterContext.Current );

	assert( node.IsLeafNode );

	return Cast< const OctreeLeaf& >( node );
}

// ================================ //
//
const VoxelAttributes&	RaycasterCPU::GetLeafAttributes		( const OctreeLeaf& leaf, RaycasterContext& raycasterContext ) const
{
	const BlockDescriptor& blockDescriptor = GetBlockDescriptor	( leaf, raycasterContext );
	uint32 attributeOffset = blockDescriptor.AttributesOffset + leaf.AttributesOffset;
	
	return Cast< const VoxelAttributes& >( raycasterContext.Octree->AccessOctree()[ attributeOffset ] );
}

// ================================ //
//
const BlockDescriptor&	RaycasterCPU::GetBlockDescriptor	( const OctreeLeaf& leaf, RaycasterContext& raycasterContext ) const
{
	return Cast< const BlockDescriptor& >( raycasterContext.Octree->AccessOctree()[ 0 ] );
}

// ================================ //
//
bool					RaycasterCPU::IsEmpty				( const OctreeNode& node )
{
	assert( !node.IsLeafNode );
	assert( !node.IndirectPtr );

	// If child mask is zero then node is empty.
	return !node.ChildMask;
}

// ================================ //
//
bool					RaycasterCPU::IsRayOutside			( ChildFlag childFlag )
{
	return childFlag >= PositiveOUT;
}

// ================================ //
//
ChildFlag				RaycasterCPU::ComputeNextChildFlag	( ChildFlag curFlag, StepDirection stepAxis )
{
	return StepTable[ curFlag ][ stepAxis ];
}

/**@detail
Children nodes are placed in memory after each other. ChildPackPtr in parent node points to the first existing
node. To find rest of nodes we must count non empty bits in child mask lying before node, we are looking for.
This function inverses this process and computes child flag from known offset.
*/
ChildFlag				RaycasterCPU::ComputeNodeFlag		( uint32 parent, uint32 current, OctreePtr& octree )
{
	const OctreeNode& parentNode = octree->GetNode( parent );

	if( !parentNode.IndirectPtr )
	{
		uint32 offset = current - parent;
		uint8 nodeNum = offset - parentNode.ChildPackPtr;		// Children are placed together.

		return FindNodeFlag( parentNode.ChildMask, nodeNum );
	}
	else
	{
		assert( !"Implement me" );
	}

	return ChildFlag();
}

// ================================ //
//
ChildFlag				RaycasterCPU::FindNodeFlag			( uint8 childMask, uint8 nodeNum )
{
	uint8 numNodes = 0;
	uint8 shift = 0;
	
	// Count bits until we will find nodeNUm of nodes.
	while( numNodes < nodeNum )
	{
		numNodes += ( childMask >> shift ) & 0x1;
		shift++;
	}

	return shift;
}

/**@brief Finds new current node and writes his absolut offset value to RaycasterContext.*/
const OctreeNode&		RaycasterCPU::SetCurrentNode		( uint32 parent, ChildFlag newChild, RaycasterContext& raycasterContext )
{
	const OctreeNode& parentNode = raycasterContext.Octree->GetNode( parent );

	uint8 numNodesBefore = CountNodesBefore( newChild, parentNode.ChildMask );
	uint32 currOffset = parent + parentNode.ChildPackPtr + numNodesBefore;

	raycasterContext.Current = currOffset;

	return raycasterContext.Octree->GetNode( currOffset );
}

// ================================ //
//
uint8					RaycasterCPU::CountNodesBefore		( ChildFlag childFlag, uint8 childMask )
{
	uint8 mask = 0xFF;
	mask = mask << childFlag;
	mask = ~mask;

	uint8 nodesBefore = childMask & mask;

	// Count bits set in nodesBefore variable.
	uint8 numNodesBefore = 0;
	
	for( uint8 i = 0; i < 8; i++ )
		numNodesBefore += ( nodesBefore >> i ) & 0x1;

	return numNodesBefore;
}

// ================================ //
//
DirectX::XMFLOAT3		RaycasterCPU::ParamLine				( DirectX::XMFLOAT3& coords, RaycasterContext& rayCtx )
{
	return ParamLine( coords, rayCtx.tCoeff, rayCtx.tBias );
}

// ================================ //
//
DirectX::XMFLOAT3		RaycasterCPU::ParamLine				( DirectX::XMFLOAT3& coords, DirectX::XMFLOAT3& tCoeff, DirectX::XMFLOAT3& tBias )
{
	XMFLOAT3 result;
	result.x = tCoeff.x * coords.x - tBias.x;
	result.y = tCoeff.y * coords.y - tBias.y;
	result.z = tCoeff.z * coords.z - tBias.z;

	return result;
}

// ================================ //
//
float					RaycasterCPU::ParamLineX			( float posX, RaycasterContext& rayCtx )
{
	return rayCtx.tCoeff.x * posX - rayCtx.tBias.x;
}

// ================================ //
//
float					RaycasterCPU::ParamLineY			( float posY, RaycasterContext& rayCtx )
{
	return rayCtx.tCoeff.y * posY - rayCtx.tBias.y;
}

// ================================ //
//
float					RaycasterCPU::ParamLineZ			( float posZ, RaycasterContext& rayCtx )
{
	return rayCtx.tCoeff.z * posZ - rayCtx.tBias.z;
}

// ================================ //
//
float					RaycasterCPU::Min					( DirectX::XMFLOAT3& coords )
{
	return fminf( fminf( coords.x, coords.y ), coords.z );
}

// ================================ //
//
bool					RaycasterCPU::ExistsChild			( const OctreeNode* node, ChildFlag childShift )
{
	return ( node->ChildMask & ( 0x1 << childShift ) ) != 0;
}

// ================================ //
//
bool					RaycasterCPU::IsLeaf				( const OctreeNode* node )
{
	return node->IsLeafNode;
}

// ================================ //
//
bool					RaycasterCPU::IsIndirectPointer		( const OctreeNode* node )
{
	return node->IndirectPtr;
}

// ================================ //
//
uint32					RaycasterCPU::GetIndirectPtr		( RaycasterContext& rayCtx, const OctreeNode* node )
{
	uint32 firstIndirect = (uint32)rayCtx.Octree->GetFirstFreeIndirect();
	return node->ChildPackPtr + firstIndirect;
}

// ================================ //
//
void					RaycasterCPU::PushOnStack			( RaycasterContext& rayCtx, uint32 idx, uint32 node, float tMax )
{
	auto& stack = rayCtx.NodesStack;
	stack[ idx ] = StackElement( node, tMax );
}

// ================================ //
//
StackElement			RaycasterCPU::ReadStack				( RaycasterContext& rayCtx, uint32 idx )
{
	auto& stack = rayCtx.NodesStack;
	return stack[ idx ];
}

//====================================================================================//
//			Render target manipulation	
//====================================================================================//

// ================================ //
//
void			RaycasterCPU::ReallocateRenderBuffer	( uint16 newWidth, uint16 newHeight )
{
	m_width = newWidth;
	m_height = newHeight;

	auto numThreads = GetNumThreads();
	auto numPixels = m_height * m_width;
	auto pixPerThread = numPixels / numThreads;

	m_renderBuffer = std::unique_ptr< uint32[] >( new uint32[ numPixels ] );

	uint32 bufferOffset = 0;
	for( auto& threadData : m_threadData )
	{
		threadData.Buffer = m_renderBuffer.get();
		threadData.StartRange = bufferOffset;
		threadData.EndRange = bufferOffset + pixPerThread;

		bufferOffset += pixPerThread;
	}

	// If number of pixels can't be devided by number of threads, we add remaining pixels to last thread.
	m_threadData.back().EndRange += numPixels % numThreads;
}

// ================================ //
//
void			RaycasterCPU::UpdateRenderTarget		( uint32* buffer, RenderTargetObject* svoRenderTarget )
{
	TextureObject* tex = svoRenderTarget->GetColorBuffer();
	auto result = tex->UpdateData( (uint8*)buffer, 1, 0 );

	assert( result );
}


}	// vr
