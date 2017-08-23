#include "RaycasterGPU.h"




namespace vr
{





// ================================ //
//
void				RaycasterGPU::Render			( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )
{
	if( m_lastOctree != octree )
		UpdateOctree( octree );




}

// ================================ //
//
void				RaycasterGPU::Init				( IRenderer* renderer, ResourceManager* resourceManager )
{
	m_resourceManager = resourceManager;
}

// ================================ //
//
void				RaycasterGPU::ProcessInput		( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )
{}


//====================================================================================//
//			Data transfer to GPU	
//====================================================================================//

// ================================ //
//
void				RaycasterGPU::UpdateOctree		( OctreePtr octree )
{
	assert( !"Implement me" );
}

}	// vr


