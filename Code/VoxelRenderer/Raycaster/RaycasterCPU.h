#pragma once


#include "IRaycaster.h"


namespace vr
{




/**@brief */
class RaycasterCPU : public IRaycaster
{
private:
protected:
public:
	explicit		RaycasterCPU		() = default;
					~RaycasterCPU		() = default;


	virtual void	Render				( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera ) override;
};


}

