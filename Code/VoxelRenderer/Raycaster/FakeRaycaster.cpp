#include "FakeRaycaster.h"

#include "swGraphicAPI/Resources/ResourcesFactory.h"

#include "VoxelRenderer/Application/RenderingHelpers.h"

namespace vr
{


// ================================ //
//
void			FakeRaycaster::Init			( IRenderer* renderer, ResourceManager* resourceManager )
{
	m_renderer = renderer;
	m_resourceManager = resourceManager;

	ShaderInputLayout* layout = nullptr;

	m_vertexShader = m_resourceManager->LoadVertexShader( L"Shaders/RendererTest.hlsl", "mainVS" );
	m_pixelShader = m_resourceManager->LoadPixelShader( L"Shaders/RendererTest.hlsl", "mainPS" );

	assert( m_vertexShader );
	assert( m_pixelShader );

	m_blendingState = resourceManager->GetBlendingState( BlendingInfo() );
	m_rasterizerState = resourceManager->GetRasterizerState( RasterizerStateInfo() );
	m_depthStencilState = resourceManager->GetDepthStencilState(  DepthStencilInfo() );

	assert( m_blendingState );
	assert( m_rasterizerState );
	assert( m_depthStencilState );
}


// ================================ //
//
void			FakeRaycaster::Render		( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )
{
	RenderingHelper::ClearRenderTargetAndDepth( m_renderer, svoRenderTarget, DirectX::XMFLOAT4( 0.0, 0.0, 0.0, 0.0 ), 1.0f );
	RenderingHelper::SetRenderTarget( m_renderer, svoRenderTarget, m_rasterizerState.Ptr(), m_blendingState.Ptr(), m_depthStencilState.Ptr() );

	SetShaderStateCommand shaderState;
	shaderState.VertexShader = m_vertexShader.Ptr();
	shaderState.PixelShader = m_pixelShader.Ptr();
	
	for( auto& tex : shaderState.Textures )
		tex = nullptr;

	m_renderer->SetShaderState( shaderState );

	RenderingHelper::DrawBufferLess( m_renderer, 3, PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

}	// vr
