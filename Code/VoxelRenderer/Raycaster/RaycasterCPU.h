#pragma once


#include "IRaycaster.h"


namespace vr
{




/**@brief */
class RaycasterCPU : public IRaycaster
{
private:
	IRenderer*				m_renderer;
	ResourceManager*		m_resourceManager;

protected:
public:
	explicit		RaycasterCPU		() = default;
					~RaycasterCPU		() = default;


	virtual void	Render				( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )		override;
	virtual void	Init				( IRenderer* renderer, ResourceManager* resourceManager )							override;
};


}

