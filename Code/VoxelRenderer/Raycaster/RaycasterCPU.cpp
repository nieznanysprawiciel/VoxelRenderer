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

// ================================ //
//
int				HighestBitPos	( int diffs )
{
	// Note: Conversion to float normalizes this float. We can read position of highest bit from exponent.
	// So we shift this float by 23 bits and subtract 127 (exponent bias).
	return ( FloatAsInt( (float)diffs ) >> 23 ) - 127;
}

// ================================ //
//
DirectX::XMFLOAT3	operator-( const DirectX::XMFLOAT3& float3 )
{
	DirectX::XMFLOAT3 result;
	result.x = -float3.x;
	result.y = -float3.y;
	result.z = -float3.z;
	return result;
}



using namespace DirectX;

// ================================ //
//
RaycasterCPU::RaycasterCPU()
	:	m_height( 0 )
	,	m_width( 0 )
	,	m_renderer( nullptr )
	,	m_resourceManager( nullptr )
	,	m_end( false )
	,	m_raycastEndBarrier( GetNumThreads() )
	,	m_raycastLoopBarrier( GetNumThreads() )
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
	// For all pixels in range for this thread.
	for( uint32 pix = data.StartRange; pix < data.EndRange; ++pix )
	{
		RaycasterContext rayCtx;
		rayCtx.Octree = data.Octree;
		rayCtx.NodesStack.resize( rayCtx.Octree->GetMaxDepth() + 1 );
		
		// Find starting position
		DirectX::XMFLOAT3 direction = ComputeRayDirection( data.Camera, pix % m_width, m_height - pix / m_width );
		DirectX::XMFLOAT3 position = ComputeRayPosition( data.Camera, pix % m_width, m_height - pix / m_width );


		InitRaycasting( position, direction, rayCtx );
		CastRay( rayCtx );


		// Shading
		if( rayCtx.Scale >= rayCtx.Octree->GetMaxDepth() )
			data.Buffer[ pix ] = DirectX::PackedVector::XMCOLOR( 0.0, 0.0, 0.0, 0.0 );
		else
		{
			OctreeLeaf leaf = GetResultLeafNode( rayCtx );
			const VoxelAttributes& attributes = GetLeafAttributes( leaf, rayCtx );

			// Fill pixel
			data.Buffer[ pix ] = attributes.Color.c;
		}
	}

	// All threads must end before buffer will be furthr processed.
	m_raycastEndBarrier.ArriveAndWait();
}

// ================================ //
//
uint16			RaycasterCPU::GetNumThreads() const
{
	//return (uint16)std::thread::hardware_concurrency();
#ifdef _DEBUG
	return 1;
#else
	return (uint16)std::thread::hardware_concurrency();
#endif // DEBUG
}

//====================================================================================//
//			Raycasting functions	
//====================================================================================//

// ================================ //
//
DirectX::XMFLOAT3		RaycasterCPU::ComputeRayPosition		( CameraActor* camera, int screenX, int screenY )
{
	auto cameraData = camera->GetCameraData();

	if( cameraData.IsPerspective )
	{
		return cameraData.Position;
	}
	else
	{
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
}

// ================================ //
//
DirectX::XMFLOAT3		RaycasterCPU::ComputeRayDirection		( CameraActor* camera, int screenX, int screenY )
{
	
	auto cameraData = camera->GetCameraData();
	if( cameraData.IsPerspective )
	{
		auto d = cameraData.Width / ( 2 * tan( XMConvertToRadians( cameraData.Fov / 2.0f ) ) );
		XMVECTOR cameraAxis = cameraData.GetDirection() * XMVectorReplicate( d );

		cameraAxis = XMVectorScale( cameraData.GetUpVector(), cameraData.Height / 2 - screenY ) + cameraAxis;
		cameraAxis = XMVectorScale( cameraData.GetRightVector(), screenX - cameraData.Width / 2 ) + cameraAxis;

		XMVECTOR rayDir = XMVector3Normalize( cameraAxis );

		XMFLOAT3 resultDir;
		XMStoreFloat3( &resultDir, rayDir );
		return resultDir;
	}
	else
	{
		return cameraData.Direction;
	}
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

	// Enable culling.
	rayCtx.tMin = fmaxf( rayCtx.tMin, 0.0f );
	rayCtx.tMax = fminf( rayCtx.tMax, 10000.0f );
	rayCtx.tCubeMin = rayCtx.tMin;

	rayCtx.Current = rayCtx.Octree->GetRootNodeOffset();
	rayCtx.Scale = rayCtx.Octree->GetMaxDepth() - 1;
	rayCtx.ScaleExp = 0.5f;
	rayCtx.ChildIdx = 0;
	rayCtx.Position = XMFLOAT3( 1.0f, 1.0f, 1.0f );

	// We must decide in which voxel we are in first step. Check which half of voxel is intersected along each axis.
	if( ParamLineX( 1.5f, rayCtx ) > rayCtx.tMin ) rayCtx.ChildIdx ^= 1, rayCtx.Position.x = 1.5f;
	if( ParamLineY( 1.5f, rayCtx ) > rayCtx.tMin ) rayCtx.ChildIdx ^= 2, rayCtx.Position.y = 1.5f;
	if( ParamLineZ( 1.5f, rayCtx ) > rayCtx.tMin ) rayCtx.ChildIdx ^= 4, rayCtx.Position.z = 1.5f;
}

// ================================ //
//
void					RaycasterCPU::CastRay				( RaycasterContext& rayCtx )
{
	rayCtx.ChildDescriptor = nullptr;


	while( rayCtx.Scale < rayCtx.Octree->GetMaxDepth() )
	{
		if( !rayCtx.ChildDescriptor )
			rayCtx.ChildDescriptor = &rayCtx.Octree->GetNode( rayCtx.Current );

		// Terminate.
		if( IsLeaf( rayCtx.ChildDescriptor ) )
			break;

		// Compute t-value in which ray leaves current voxel.
		XMFLOAT3 corner = ParamLine( rayCtx.Position, rayCtx );
		float tLeave = Min( corner );

		ChildFlag childShift = rayCtx.ChildIdx ^ rayCtx.OctantMask; // permute child slots based on the mirroring

		if( ExistsChild( rayCtx.ChildDescriptor, childShift ) && rayCtx.tMin <= rayCtx.tMax )
		{
			float tVoxelMax = fminf( rayCtx.tMax, tLeave );

			if( rayCtx.tMin <= tVoxelMax )
			{
				PushStep( rayCtx, corner, tVoxelMax, childShift );
				continue;
			}
		}

		ChildFlag childIdxChange = AdvanceStep( rayCtx, corner, tLeave );

		if( IsRayOutside( rayCtx.ChildIdx, childIdxChange ) )
			PopStep( rayCtx, childIdxChange );
	}

	if( rayCtx.Scale >= rayCtx.Octree->GetMaxDepth() )
		rayCtx.tMin = 2.0f;

	rayCtx.Depth = rayCtx.tMin - rayCtx.tCubeMin;

	if( ( rayCtx.OctantMask & 1 ) == 0 ) rayCtx.Position.x = 3.0f - rayCtx.ScaleExp - rayCtx.Position.x;
	if( ( rayCtx.OctantMask & 2 ) == 0 ) rayCtx.Position.y = 3.0f - rayCtx.ScaleExp - rayCtx.Position.y;
	if( ( rayCtx.OctantMask & 4 ) == 0 ) rayCtx.Position.z = 3.0f - rayCtx.ScaleExp - rayCtx.Position.z;

}

// ================================ //
//
ChildFlag				RaycasterCPU::AdvanceStep			( RaycasterContext& rayCtx, const DirectX::XMFLOAT3& corner, float tLeave )
{
	// ADVANCE
	// Step along the ray.

	// Check which voxel boundaries we crossed and set proper flag. Advance position by current voxel size.
	ChildFlag childIdxChange = 0;
	if( corner.x <= tLeave ) childIdxChange ^= 1, rayCtx.Position.x -= rayCtx.ScaleExp;
	if( corner.y <= tLeave ) childIdxChange ^= 2, rayCtx.Position.y -= rayCtx.ScaleExp;
	if( corner.z <= tLeave ) childIdxChange ^= 4, rayCtx.Position.z -= rayCtx.ScaleExp;

	// Update active t-span and flip bits of the child slot index.

	rayCtx.tMin = tLeave;
	rayCtx.ChildIdx ^= childIdxChange;

	return childIdxChange;
}

// ================================ //
//
void					RaycasterCPU::PopStep				( RaycasterContext& rayCtx, ChildFlag childIdxChange )
{
	// POP
	// Find the highest differing bit between the two positions.

	rayCtx.Scale = FindNewHierarchyLevel( rayCtx.Position, rayCtx.ScaleExp, childIdxChange );
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
	rayCtx.ChildDescriptor = nullptr;
}

// ================================ //
//
void					RaycasterCPU::PushStep				( RaycasterContext& rayCtx, const DirectX::XMFLOAT3& corner, float tVoxelMax, ChildFlag childShift )
{
	float half = rayCtx.ScaleExp * 0.5f;				// Half of current node cube dimmension.

	// This line computes intersection with center of current voxel. Write full equation to see why this works.
	XMFLOAT3 tCenter = ParamLine( XMFLOAT3( half, half, half ), rayCtx.tCoeff, -corner );

	PushOnStack( rayCtx, rayCtx.Scale, rayCtx.Current, rayCtx.tMax );

	rayCtx.Current = ComputeChildOffset( rayCtx, rayCtx.ChildDescriptor, childShift );


	// Select child voxel that the ray enters first.
	rayCtx.ChildIdx = 0;
	rayCtx.Scale--;
	rayCtx.ScaleExp = half;

	if( tCenter.x > rayCtx.tMin ) rayCtx.ChildIdx ^= 1, rayCtx.Position.x += rayCtx.ScaleExp;
	if( tCenter.y > rayCtx.tMin ) rayCtx.ChildIdx ^= 2, rayCtx.Position.y += rayCtx.ScaleExp;
	if( tCenter.z > rayCtx.tMin ) rayCtx.ChildIdx ^= 4, rayCtx.Position.z += rayCtx.ScaleExp;

	// Update active t-span and invalidate cached child descriptor.

	rayCtx.tMax = tVoxelMax;
	rayCtx.ChildDescriptor = nullptr;
}


// ================================ //
//
const OctreeLeaf&		RaycasterCPU::GetResultLeafNode		( RaycasterContext& raycasterContext ) const
{
	const OctreeNode& node = raycasterContext.Octree->GetNode( raycasterContext.Current );

	assert( node.IsLeafNode() );

	return Cast< const OctreeLeaf& >( node );
}

// ================================ //
//
const VoxelAttributes&	RaycasterCPU::GetLeafAttributes		( const OctreeLeaf& leaf, RaycasterContext& raycasterContext ) const
{
	const BlockDescriptor& blockDescriptor = GetBlockDescriptor	( leaf, raycasterContext );
	uint32 attributeOffset = blockDescriptor.AttributesOffset + leaf.AttributesOffset();
	
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
	assert( !node.IsLeafNode() );
	assert( !node.IsIndirectPtr() );

	// If child mask is zero then node is empty.
	return !node.ChildMask();
}

// ================================ //
//
bool					RaycasterCPU::IsRayOutside			( ChildFlag childIdx, ChildFlag childIdxChange )
{
	// If the bit flips disagree with the ray direction, ray is outside of current voxel.
	return ( childIdx & childIdxChange ) != 0;
}


// ================================ //
//
uint8					RaycasterCPU::CountNodesBefore		( ChildFlag childShift, uint8 childMask )
{
	// childShift represents index of child voxel in childMask counting from highest bit.
	uint8 mask = 0x7F;											// Selects all bits except first in childMask.
	uint8 nodesBefore = ( childMask << childShift ) & mask;		// Shift all children bits in such way that our current xhild will be on first place.

	// Count bits set in nodesBefore variable.
	uint8 numNodesBefore = 0;
	
	for( uint8 i = 0; i < 7; i++ )
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
	return ( node->ChildMask() & ( 0x80 >> childShift ) ) != 0;
}

// ================================ //
//
bool					RaycasterCPU::IsLeaf				( const OctreeNode* node )
{
	return node->IsLeafNode();
}

// ================================ //
//
bool					RaycasterCPU::IsIndirectPointer		( const OctreeNode* node )
{
	return node->IsIndirectPtr();
}

// ================================ //
//
uint32					RaycasterCPU::GetIndirectPtr		( RaycasterContext& rayCtx, const OctreeNode* node )
{
	const vr::OctreeFarPointer& farPointer = Cast< const vr::OctreeFarPointer& >( rayCtx.Octree->GetNode( node->ChildPackPtr() ) );
	return farPointer.Offset;
}

// ================================ //
//
uint32					RaycasterCPU::ComputeChildOffset	( RaycasterContext& rayCtx, const OctreeNode* node, ChildFlag childShift )
{
	// Find child descriptor corresponding to the current voxel.
	uint32 childOffset = 0;
	if( IsIndirectPointer( node ) ) // far
	{
		childOffset = GetIndirectPtr( rayCtx, node );
	}
	else
	{
		childOffset = rayCtx.Current + node->ChildPackPtr();
	}

	childOffset += CountNodesBefore( childShift, node->ChildMask() );
	return childOffset;
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

// ================================ //
//
uint32					RaycasterCPU::FindNewHierarchyLevel	( DirectX::XMFLOAT3& position, float scaleExp, ChildFlag childIdxChange )
{
	// During tree traversal, ray can stay inside the same voxel and step to neighbor child or it can leave voxel.
	// In second case we must go up in the voxels hierarchy (sometimes even few levels up).
	// This function finds, how many octree levels we must leave.
	// This code checks differing bits in float representation of position before step and after.
	// Remember that positions can be only negatives powers of 1.
	// The highest bit will say how much we must go upside.
	uint32 differingBits = 0;
	if( ( childIdxChange & 1 ) != 0 ) differingBits |= FloatAsInt( position.x ) ^ FloatAsInt( position.x + scaleExp );
	if( ( childIdxChange & 2 ) != 0 ) differingBits |= FloatAsInt( position.y ) ^ FloatAsInt( position.y + scaleExp );
	if( ( childIdxChange & 4 ) != 0 ) differingBits |= FloatAsInt( position.z ) ^ FloatAsInt( position.z + scaleExp );

	return HighestBitPos( differingBits );
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
