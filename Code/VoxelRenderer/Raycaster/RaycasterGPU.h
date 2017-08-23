#pragma once


#include "IRaycaster.h"



namespace vr
{

/**@brief Bit shifts for specific position in child mask in OctreeNode.*/
typedef uint8 ChildFlag;



/**@brief GPU raycaster using compute shaders.*/
class RaycasterGPU : public IRaycaster
{
protected:

	IRenderer*				m_renderer;
	ResourceManager*		m_resourceManager;

	OctreePtr				m_lastOctree;

public:



	virtual void			Render			( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )			override;
	virtual void			Init			( IRenderer* renderer, ResourceManager* resourceManager )								override;
	virtual void			ProcessInput	( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )		override;


private:

	void					UpdateOctree	( OctreePtr octree );
};


}	// vr

