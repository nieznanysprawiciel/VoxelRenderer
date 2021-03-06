#include "RaycasterGPU.h"

#include "VoxelRenderer/Application/RenderingHelpers.h"

#include "Structures/ShaderCameraBuffer.h"


using namespace DirectX;
using namespace sw::input;



namespace vr
{



//====================================================================================//
//			Rendering	
//====================================================================================//



// ================================ //
//
void				RaycasterGPU::Render			( TimeType time, OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )
{
	UpdateOctree( octree );
	UpdateCamera( camera );
	UpdateLights();

	RenderingHelper::ClearRenderTargetAndDepth( m_renderer, svoRenderTarget, DirectX::XMFLOAT4( 0.0, 0.0, 0.0, 0.0 ), 1.0f );
	RenderingHelper::SetRenderTarget( m_renderer, svoRenderTarget, m_rasterizerState.Ptr(), m_blendingState.Ptr(), m_depthStencilState.Ptr() );

	SetShaderStateCommand shaderState;
	RenderingHelper::ClearTextureState( shaderState );

	shaderState.VertexShader = m_vertexShader.Ptr();
	shaderState.PixelShader = m_pixelShader.Ptr();

	RenderingHelper::SetTexture( shaderState, m_octreeTexBuff.Ptr(), 0, (uint8)ShaderType::PixelShader );

	m_renderer->SetShaderState( shaderState );

	RenderingHelper::BindBuffer( m_renderer, m_cameraBuffer.Ptr(), 0, (uint8)ShaderType::PixelShader );
	RenderingHelper::BindBuffer( m_renderer, m_lightsBuffer.Ptr(), 1, (uint8)ShaderType::PixelShader );

	RenderingHelper::DrawBufferLess( m_renderer, 3, PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}

// ================================ //
//
void				RaycasterGPU::Init				( IRenderer* renderer, ResourceManager* resourceManager, LightModulePtr lights )
{
	m_resourceManager = resourceManager;
	m_renderer = renderer;
	m_lights = lights;

	m_blendingState = resourceManager->GetBlendingState( BlendingInfo() );
	m_rasterizerState = resourceManager->GetRasterizerState( RasterizerStateInfo() );
	m_depthStencilState = resourceManager->GetDepthStencilState( DepthStencilInfo() );

	m_vertexShader = resourceManager->LoadVertexShader( L"Shaders/FullscreenTriangleVS.hlsl", "main" );
	m_pixelShader = resourceManager->LoadPixelShader( L"Shaders/RaycasterGPU/SimpleColor.hlsl", "main" );
}

// ================================ //
//
void				RaycasterGPU::ProcessInput		( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )
{
	if( keyboard[ Keyboard::PhysicalKeys::KEY_1 ].IsKeyDownEvent() )
	{
		m_pixelShader = m_resourceManager->LoadPixelShader( L"Shaders/RaycasterGPU/SimpleColor.hlsl", "main" );
	}
	else if( keyboard[ Keyboard::PhysicalKeys::KEY_2 ].IsKeyDownEvent() )
	{
		m_pixelShader = m_resourceManager->LoadPixelShader( L"Shaders/RaycasterGPU/Normals.hlsl", "main" );
	}
	else if( keyboard[ Keyboard::PhysicalKeys::KEY_3 ].IsKeyDownEvent() )
	{
		m_pixelShader = m_resourceManager->LoadPixelShader( L"Shaders/RaycasterGPU/PhongColor.hlsl", "main" );
	}
}


//====================================================================================//
//			Data transfer to GPU	
//====================================================================================//

// ================================ //
//
void				RaycasterGPU::UpdateOctree		( OctreePtr octree )
{
	if( m_lastOctree != octree )
	{
		m_lastOctree = octree;

		TextureBufferInitData bufferDesc;
		bufferDesc.AllowRaw = true;
		bufferDesc.ElementSize = sizeof( OctreeNode );
		bufferDesc.NumElements = (uint32)octree->AccessOctree().size();
		bufferDesc.Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;
		bufferDesc.Data = (uint8*)octree->AccessOctree().data();
		
		// We should release old octree here.

		m_octreeBuffer = m_resourceManager->CreateTextureBuffer( L"Octree", bufferDesc );
		m_octreeTexBuff = m_octreeBuffer->CreateRawShaderView( L"RawOctreeView", m_resourceManager );
	}
}

// ================================ //
//
void				RaycasterGPU::UpdateCamera		( CameraActor* camera )
{
	CameraData data = camera->GetCameraData();

	ShaderCameraData cameraData;
	cameraData.Direction = data.Direction;
	cameraData.FarPlane = data.FarPlane;
	cameraData.Fov = data.Fov;
	cameraData.Height = data.Height;
	cameraData.IsPerspective = data.IsPerspective;
	cameraData.NearPlane = data.NearPlane;
	cameraData.Position = data.Position;
	cameraData.RightVector = data.RightVector;
	cameraData.UpVector = data.UpVector;
	cameraData.ViewportSize = data.ViewportSize;
	cameraData.Width = data.Width;

	if( !m_cameraBuffer )
		m_cameraBuffer = m_resourceManager->CreateConstantsBuffer( L"Camera constant buffer", (uint8*)&cameraData, sizeof( ShaderCameraData ) );
	else
		RenderingHelper::UpdateBuffer( m_renderer, m_cameraBuffer.Ptr(), cameraData );

}

// ================================ //
//
void				RaycasterGPU::UpdateLights		()
{
	if( !m_lightsBuffer )
		m_lightsBuffer = m_resourceManager->CreateConstantsBuffer( L"Lights constant buffer", (uint8*)&m_lights->GetLights(), sizeof( LightConstants ) );
	else
		RenderingHelper::UpdateBuffer( m_renderer, m_lightsBuffer.Ptr(), m_lights->GetLights() );
}

}	// vr


