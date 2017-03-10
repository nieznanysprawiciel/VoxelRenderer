#pragma once


#include "swGraphicAPI/ResourceManager/ResourceManager.h"
#include "swGraphicAPI/Resources/MeshResources.h"
#include "swGraphicAPI/Rendering/IRenderer.h"


namespace vr
{

/**@brief */
class BlitEffect
{
private:
protected:

	ResourcePtr< VertexShader >		m_blitVS;
	ResourcePtr< PixelShader >		m_blitPS;

	ResourcePtr< BlendingState >		m_blendingState;
	ResourcePtr< RasterizerState >		m_rasterizerState;
	ResourcePtr< DepthStencilState >	m_depthStencilState;

public:
	explicit		BlitEffect		( ResourceManager* resourceManager );
					~BlitEffect		() = default;

	void			Blit			( IRenderer* renderer, TextureObject* source, RenderTargetObject* destRT );
};

DEFINE_UPTR_TYPE( BlitEffect );


}
