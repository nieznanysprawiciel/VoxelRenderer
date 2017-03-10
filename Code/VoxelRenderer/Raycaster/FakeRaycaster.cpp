#include "FakeRaycaster.h"

#include "swGraphicAPI/Resources/ResourcesFactory.h"

#include "VoxelRenderer/Application/RenderingHelpers.h"

namespace vr
{

auto LAYOUT_EMPTY_DESC = ResourcesFactory::CreateInputLayoutDescriptor( L"::Empty" );


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

	m_blendingState = m_resourceManager->CreateBlendingState( L"::DefaultBlendingState", BlendingInfo() );
	m_rasterizerState = m_resourceManager->CreateRasterizerState( L"::DefaultRasterizerState", RasterizerStateInfo() );
	m_depthStencilState = m_resourceManager->CreateDepthStencilState( L"::DefaultDepthStencilState", DepthStencilInfo() );

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
