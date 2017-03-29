#include "RaycasterCPU.h"


#include <iostream>


namespace vr
{


// ================================ //
//
RaycasterCPU::RaycasterCPU()
	:	m_height( 0 )
	,	m_width( 0 )
	,	m_renderer( nullptr )
	,	m_resourceManager( nullptr )
	,	m_run( false )
	,	m_end( false )
{}

// ================================ //
//
void			RaycasterCPU::Init			( IRenderer* renderer, ResourceManager* resourceManager )
{
	PrepareThreads();

}


// ================================ //
//
void			RaycasterCPU::Render		( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )
{
	if( m_width != camera->GetWidth() || m_height != camera->GetHeight() )
		ReallocateRenderBuffer( camera->GetWidth(), camera->GetHeight() );

	SpawnThreads( octree, camera );
	UpdateRenderTarget( m_renderBuffer.get(), svoRenderTarget );
}

// ================================ //
//
void			RaycasterCPU::SpawnThreads				( OctreePtr octree, CameraActor* camera )
{
	// Run threads
	m_run = true;
	m_condition.notify_all();

	m_run = false;
}

// ================================ //
//
void			RaycasterCPU::ThreadWait()
{
	while( !m_run )
		m_condition.wait( m_lock );

	// Check it to be sure.
	if( !m_run )
		std::cout << "Spurious wake up!\n";
}

// ================================ //
//
void			RaycasterCPU::RaycasterThread			( OctreePtr octree, CameraActor* camera, uint32 startRange, uint32 endRange )
{
	ThreadWait();



}

// ================================ //
//
void			RaycasterCPU::PrepareThreads()
{
	auto numThreads = std::thread::hardware_concurrency();
	assert( numThreads );
	m_threadPool.reserve( numThreads );
}

// ================================ //
//
void			RaycasterCPU::ReallocateRenderBuffer	( uint16 newWidth, uint16 newHeight )
{
	m_width = newWidth;
	m_height = newHeight;

	m_renderBuffer = std::unique_ptr< uint32[] >( new uint32[ m_height * m_width ] );
}

// ================================ //
//
void			RaycasterCPU::UpdateRenderTarget		( uint32* buffer, RenderTargetObject* svoRenderTarget )
{
	assert( false );
}


}	// vr
