#pragma once


#include "IRaycaster.h"

#include "swGraphicAPI/Resources/MeshResources.h"


namespace vr
{




/**@brief */
class FakeRaycaster : public IRaycaster
{
private:
	
	IRenderer*				m_renderer;
	ResourceManager*		m_resourceManager;

	ResourcePtr< VertexShader >		m_vertexShader;
	ResourcePtr< PixelShader >		m_pixelShader;


protected:
public:
	explicit		FakeRaycaster		() = default;
					~FakeRaycaster		() = default;


	virtual void	Render				( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )					override;
	virtual void	Init				( IRenderer* renderer, ResourceManager* resourceManager )										override;
};


}

