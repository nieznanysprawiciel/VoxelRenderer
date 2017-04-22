#include "RaycasterCPU.h"

#include "swCommonLib/Common/Multithreading/ThreadsLatch.h"


#include <iostream>


namespace vr
{


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
	auto numThreads = std::thread::hardware_concurrency();
	assert( numThreads );
	m_threadPool.reserve( numThreads - 1 );		// Note: GUI thread will work too.
	m_threadData.resize( numThreads );
}

//====================================================================================//
//			Raycaster core	
//====================================================================================//

// ================================ //
//
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

		// Find starting position
		rayCtx.RayDirection = ComputeRayDirection( data.Camera, pix % m_width, pix / m_width );
		DirectX::XMFLOAT3 position = ComputeRayPosition( data.Camera, pix % m_width, pix / m_width );
		const OctreeNode& startNode = FindStartingNode( position, rayCtx.RayDirection, rayCtx );

		// Raycast octree
		bool run = true;
		while( run )
		{
			if( rayCtx.tMax.x < rayCtx.tMax.y )
			{
				if( rayCtx.tMax.x < rayCtx.tMax.z )
				{
					run = StepX( rayCtx );
					rayCtx.tMax.x += rayCtx.tDelta.x;
				}
				else
				{
					run = StepZ( rayCtx );
					rayCtx.tMax.z += rayCtx.tDelta.z;
				}
			}
			else
			{
				if( rayCtx.tMax.y < rayCtx.tMax.z )
				{
					run = StepY( rayCtx );
					rayCtx.tMax.y += rayCtx.tDelta.y;
				}
				else
				{
					run = StepZ( rayCtx );
					rayCtx.tMax.z += rayCtx.tDelta.z;
				}
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
	return (uint16)m_threadData.size();
}

// ================================ //
//
DirectX::XMFLOAT3		RaycasterCPU::ComputeRayPosition		( CameraActor* camera, int screenX, int screenY )
{
	/// @todo perspective camera
	auto cameraData = camera->GetCameraData();
	
	float aspect = cameraData.Width / cameraData.Height;

	float xFactor = screenX / ( cameraData.Width / 2 ) - 1.0;
	float yFactor = 1.0 - screenY / ( cameraData.Height / 2 );

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
bool					RaycasterCPU::StepX			( RaycasterContext& raycasterContext )
{
	return false;
}

// ================================ //
//
bool					RaycasterCPU::StepY			( RaycasterContext& raycasterContext )
{
	return false;
}

// ================================ //
//
bool					RaycasterCPU::StepZ			( RaycasterContext& raycasterContext )
{
	return false;
}

// ================================ //
//
const OctreeLeaf&		RaycasterCPU::GetResultLeafNode		( RaycasterContext & raycasterContext ) const
{
	uint32 offsetToLeaf = raycasterContext.NodesStack.top();
	const OctreeNode& node = raycasterContext.Octree->AccessOctree()[ offsetToLeaf ];

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
