#include "BlitEffect.h"

#include "swGraphicAPI/Rendering/RenderCommands.h"
#include "VoxelRenderer/Application/RenderingHelpers.h"


namespace vr
{

BlitEffect::BlitEffect	( ResourceManager* resourceManager )
{
	m_blitVS = resourceManager->LoadVertexShader( L"Shaders/FullscreenTriangleVS.hlsl", "main" );
	m_blitPS = resourceManager->LoadPixelShader( L"Shaders/BlitPS.hlsl", "main" );

	m_blendingState = resourceManager->GetBlendingState( BlendingInfo() );
	m_rasterizerState = resourceManager->GetRasterizerState( RasterizerStateInfo() );
	m_depthStencilState = resourceManager->GetDepthStencilState(  DepthStencilInfo() );

	assert( m_blendingState );
	assert( m_rasterizerState );
	assert( m_depthStencilState );
}

// ================================ //
//
void			BlitEffect::Blit		( IRenderer* renderer, TextureObject* source, RenderTargetObject* destRT )
{
	RenderingHelper::ClearRenderTargetAndDepth( renderer, destRT, DirectX::XMFLOAT4( 0.0, 0.0, 0.0, 0.0 ), 1.0f );
	RenderingHelper::SetRenderTarget( renderer, destRT, m_rasterizerState.Ptr(), m_blendingState.Ptr(), m_depthStencilState.Ptr() );

	SetShaderStateCommand shaderState;
	RenderingHelper::ClearTextureState( shaderState );

	shaderState.VertexShader = m_blitVS.Ptr();
	shaderState.PixelShader = m_blitPS.Ptr();

	RenderingHelper::SetTexture( shaderState, source, 0, (uint8)ShaderType::PixelShader );

	renderer->SetShaderState( shaderState );

	RenderingHelper::DrawBufferLess( renderer, 3, PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}


}
