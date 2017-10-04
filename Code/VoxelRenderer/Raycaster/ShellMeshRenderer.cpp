#include "ShellMeshRenderer.h"

#include "VoxelRenderer/Application/RenderingHelpers.h"

#include "swGraphicAPI/Resources/ResourcesFactory.h"

#include "Structures/ShaderCameraBuffer.h"




using namespace DirectX;
using namespace sw::input;



namespace vr
{


//====================================================================================//
//			Helpers	
//====================================================================================//


// ================================ //
//
DirectX::XMFLOAT4X4				Transpose				( DirectX::XMFLOAT4X4 mat )
{
	auto matrix = XMLoadFloat4x4( &mat );
	matrix = DirectX::XMMatrixTranspose( matrix );
	XMStoreFloat4x4( &mat, matrix );
	return mat;
}




//====================================================================================//
//			Rendering	
//====================================================================================//



// ================================ //
//
void				ShellMeshRenderer::RenderShellMeshes( const std::vector< ShellMeshPtr >& shellMeshes, CameraActor* camera )
{
	UpdateCamera( camera );


}

// ================================ //
//
void				ShellMeshRenderer::Render			( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )
{
	//UpdateOctree( octree );
	//UpdateCamera( camera );


	//RenderingHelper::ClearRenderTargetAndDepth( m_renderer, svoRenderTarget, DirectX::XMFLOAT4( 0.0, 0.0, 0.0, 0.0 ), 1.0f );
	//RenderingHelper::SetRenderTarget( m_renderer, svoRenderTarget, m_rasterizerState.Ptr(), m_blendingState.Ptr(), m_depthStencilState.Ptr() );

	//SetShaderStateCommand shaderState;
	//RenderingHelper::ClearTextureState( shaderState );

	//shaderState.VertexShader = m_vertexShader.Ptr();
	//shaderState.PixelShader = m_pixelShader.Ptr();

	//RenderingHelper::SetTexture( shaderState, m_octreeTexBuff.Ptr(), 0, (uint8)ShaderType::PixelShader );

	//m_renderer->SetShaderState( shaderState );

	//RenderingHelper::BindBuffer( m_renderer, m_cameraBuffer.Ptr(), 0, (uint8)ShaderType::PixelShader );

	//RenderingHelper::DrawBufferLess( m_renderer, 3, PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

// ================================ //
//
void				ShellMeshRenderer::Init				( IRenderer* renderer, ResourceManager* resourceManager )
{
	m_resourceManager = resourceManager;
	m_renderer = renderer;

	m_blendingState = resourceManager->GetBlendingState( BlendingInfo() );
	m_rasterizerState = resourceManager->GetRasterizerState( RasterizerStateInfo() );
	m_depthStencilState = resourceManager->GetDepthStencilState( DepthStencilInfo() );

	// Layout
	auto animLayoutDesc = ResourcesFactory::CreateInputLayoutDescriptor( L"::AnimationLayout" );
	animLayoutDesc->AddRow( "POSITION", ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0, false, 0 );
	animLayoutDesc->AddRow( "BLENDINDICES", ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_UINT, 0, 12, false, 0 );
	animLayoutDesc->AddRow( "BLENDWEIGHT", ResourceFormat::RESOURCE_FORMAT_R32G32B32A32_FLOAT, 0, 16, false, 0 );
	
	ShaderInputLayout* layout;

	m_vertexShader = resourceManager->LoadVertexShader( L"Shaders/ShellMesh/ShellMeshVS.hlsl", "main", &layout, animLayoutDesc );
	m_pixelShader = resourceManager->LoadPixelShader( L"Shaders/ShellMesh/ShellMeshPS.hlsl", "main" );

	m_layout = layout;
}

// ================================ //
//
void				ShellMeshRenderer::ProcessInput		( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )
{}


//====================================================================================//
//			Data transfer to GPU	
//====================================================================================//


// ================================ //
//
void				ShellMeshRenderer::UpdateCamera		( CameraActor* camera )
{
	CameraData data = camera->GetCameraData();

	CameraConstants cameraData;
	cameraData.ProjectionMatrix = Transpose( camera->GetProjection() );
	cameraData.ViewMatrix = Transpose( camera->GetView() );
	cameraData.CameraPosition = data.Position;

	if( !m_cameraBuffer )
		m_cameraBuffer = m_resourceManager->CreateConstantsBuffer( L"Camera constant buffer", (uint8*)&cameraData, sizeof( CameraConstants ) );
	else
		RenderingHelper::UpdateBuffer( m_renderer, m_cameraBuffer.Ptr(), cameraData );

}

}	// vr


