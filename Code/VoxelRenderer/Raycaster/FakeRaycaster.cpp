#include "FakeRaycaster.h"



namespace vr
{



// ================================ //
//
void			FakeRaycaster::Init			( IRenderer* renderer, ResourceManager* resourceManager )
{
	m_vertexShader = m_resourceManager->LoadVertexShader( L"Shaders/RendererTest.hlsl", "mainVS" );
	m_pixelShader = m_resourceManager->LoadPixelShader( L"Shaders/RendererTest.hlsl", "mainPS" );

	assert( m_vertexShader );
	assert( m_pixelShader );
}


// ================================ //
//
void			FakeRaycaster::Render		( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )
{
	ClearRenderTargetCommand clearRT;
	clearRT.RenderTarget = svoRenderTarget;
	clearRT.ClearColor = DirectX::XMFLOAT4( 0.0, 0.0, 0.0, 0.0 );
	clearRT.DepthValue = 0.0;
	clearRT.ClearRenderTarget = true;
	clearRT.ClearDepth = true;

	m_renderer->ClearRenderTarget( clearRT );


	SetShaderStateCommand shaderState;
	shaderState.VertexShader = m_vertexShader.Ptr();
	shaderState.PixelShader = m_pixelShader.Ptr();
	
	for( auto& tex : shaderState.Textures )
		tex = nullptr;

	m_renderer->SetShaderState( shaderState );

	DrawCommand drawCmd;
	drawCmd.IndexBufer = nullptr;
	drawCmd.VertexBuffer = nullptr;
	drawCmd.ExtendedIndex = false;
	drawCmd.BaseVertex = 0;
	drawCmd.BufferOffset = 0;
	drawCmd.NumVertices = 6;
	drawCmd.Layout = nullptr;
	drawCmd.Topology = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_renderer->Draw( drawCmd );
}

}	// vr
