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

	ResourcePtr< VertexShader >			m_vertexShader;
	ResourcePtr< PixelShader >			m_pixelShader;

	ResourcePtr< BlendingState >		m_blendingState;
	ResourcePtr< RasterizerState >		m_rasterizerState;
	ResourcePtr< DepthStencilState >	m_depthStencilState;

protected:
public:
	explicit		FakeRaycaster		() = default;
					~FakeRaycaster		() = default;


	virtual void	Render				( TimeType time, OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )	override;
	virtual void	Init				( IRenderer* renderer, ResourceManager* resourceManager, LightModulePtr lights )					override;

	// Inherited via IRaycaster
	virtual void	ProcessInput		( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )				override {}
};


}

