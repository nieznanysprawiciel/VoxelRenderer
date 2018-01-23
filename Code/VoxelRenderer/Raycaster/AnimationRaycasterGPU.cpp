#include "AnimationRaycasterGPU.h"

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


namespace
{

// ================================ //
//
DirectX::XMFLOAT4X4				Transpose				( DirectX::XMFLOAT4X4 mat )
{
	auto matrix = XMLoadFloat4x4( &mat );
	matrix = DirectX::XMMatrixTranspose( matrix );
	XMStoreFloat4x4( &mat, matrix );
	return mat;
}

}




//====================================================================================//
//			Rendering	
//====================================================================================//


// ================================ //
//
AnimationRaycasterGPU::AnimationRaycasterGPU()
	:	m_height( 0 )
	,	m_width( 0 )
	,	m_pause( false )
	,	m_offsetShell( 0.0 )
	,	m_enableShellDisplay( true )
	,	m_maxError( 0.0 )
{}

// ================================ //
//
void				AnimationRaycasterGPU::RenderShellMeshes( TimeType time, const std::vector< ShellMeshPtr >& shellMeshes, CameraActor* camera )
{
	if( m_width != camera->GetWidth() || m_height != camera->GetHeight() )
		ReallocateRenderTarget( (uint16)camera->GetWidth(), (uint16)camera->GetHeight() );

	UpdateCamera( camera );

	RenderingHelper::ClearRenderTargetAndDepth( m_renderer, m_shellMeshTarget.Ptr(), DirectX::XMFLOAT4( 0.0, 0.0, 0.0, 0.0 ), 1.0f );
	RenderingHelper::SetRenderTarget( m_renderer, m_shellMeshTarget.Ptr(), m_rasterizerState.Ptr(), m_blendingState.Ptr(), m_depthStencilState.Ptr() );

	SetShaderStateExCommand shaderState;
	RenderingHelper::ClearTextureState( shaderState );

	shaderState.VertexShader = m_animVertexShader.Ptr();
	shaderState.PixelShader = m_animPixelShader.Ptr();
	shaderState.GeometryShader = m_animGeometryShader.Ptr();

	RenderingHelper::BindBuffer( m_renderer, m_cameraBuffer.Ptr(), 0, (uint8)ShaderType::VertexShader | (uint8)ShaderType::PixelShader );

	for( auto & shellMesh : shellMeshes )
	{
		UpdateAnimation( time, shellMesh );
		UpdateMeshBuffer( shellMesh );

		RenderingHelper::BindBuffer( m_renderer, m_bonesBuffer.Ptr(), 1, (uint8)ShaderType::VertexShader | (uint8)ShaderType::PixelShader );
		RenderingHelper::BindBuffer( m_renderer, m_meshTransformBuffer.Ptr(), 2, (uint8)ShaderType::VertexShader | (uint8)ShaderType::PixelShader );

		RenderingHelper::SetTexture( shaderState, shellMesh->GetOctreeTexture(), 0, (uint8)ShaderType::PixelShader );
		m_renderer->SetShaderState( shaderState );

		DrawCommand drawCommand;
		drawCommand.BaseVertex = 0;
		drawCommand.BufferOffset = 0;
		drawCommand.NumVertices = (uint32)shellMesh->GetNumIndicies();
		drawCommand.Topology = PrimitiveTopology::PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		drawCommand.VertexBuffer = shellMesh->GetVertexBuffer();
		drawCommand.IndexBufer = shellMesh->GetIndexBuffer();
		drawCommand.ExtendedIndex = true;
		drawCommand.Layout = m_layout.Ptr();

		m_renderer->Draw( drawCommand );
	}
}

// ================================ //
//
void				AnimationRaycasterGPU::Render			( TimeType time, OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )
{
	m_blitEffect->Blit( m_renderer, m_shellMeshTarget->GetColorBuffer(), svoRenderTarget );
}

// ================================ //
//
void				AnimationRaycasterGPU::Init				( IRenderer* renderer, ResourceManager* resourceManager, LightModulePtr lights )
{
	m_resourceManager = resourceManager;
	m_renderer = renderer;

	m_blendingState = resourceManager->GetBlendingState( BlendingInfo() );
	m_rasterizerState = resourceManager->GetRasterizerState( RasterizerStateInfo() );
	m_depthStencilState = resourceManager->GetDepthStencilState( DepthStencilInfo() );

	// Layout
	auto animLayoutDesc = ShellMesh::CreateShellLayout();

	ShaderInputLayout* layout;

	// Generate layout for shader that uses all vertex attributes.
	auto shader = m_resourceManager->LoadVertexShader( L"Shaders/AnimationRaycasterGPU/AnimLoadedNormalsVS.hlsl", "main", &layout, animLayoutDesc );

	m_animVertexShader = resourceManager->LoadVertexShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimation3VS.hlsl", "main" );
	m_animGeometryShader = resourceManager->LoadGeometryShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimation3GS.hlsl", "main" );
	m_animPixelShader = resourceManager->LoadPixelShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimation3PS.hlsl", "main" );

	m_layout = layout;

	m_blitEffect = std::unique_ptr< BlitEffect >( new BlitEffect( m_resourceManager ) );
}

// ================================ //
//
void				AnimationRaycasterGPU::ProcessInput		( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )
{
	if( keyboard[ Keyboard::PhysicalKeys::KEY_1 ].IsKeyDownEvent() )
	{
		m_animVertexShader = m_resourceManager->LoadVertexShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimationVS.hlsl", "main" );
		m_animGeometryShader = m_resourceManager->LoadGeometryShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimationGS.hlsl", "main" );
		m_animPixelShader = m_resourceManager->LoadPixelShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimationPS.hlsl", "main" );
	}
	else if( keyboard[ Keyboard::PhysicalKeys::KEY_2 ].IsKeyDownEvent() )
	{
		m_animVertexShader = m_resourceManager->LoadVertexShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimation2VS.hlsl", "main" );
		m_animGeometryShader = m_resourceManager->LoadGeometryShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimation2GS.hlsl", "main" );
		m_animPixelShader = m_resourceManager->LoadPixelShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimation2PS.hlsl", "main" );
	}
	else if( keyboard[ Keyboard::PhysicalKeys::KEY_3 ].IsKeyDownEvent() )
	{
		m_animVertexShader = m_resourceManager->LoadVertexShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimation2VS.hlsl", "main" );
		m_animGeometryShader = m_resourceManager->LoadGeometryShader( L"Shaders/AnimationRaycasterGPU/AnimGenNormalsGS.hlsl", "main" );
		m_animPixelShader = m_resourceManager->LoadPixelShader( L"Shaders/AnimationRaycasterGPU/AnimGenNormalsPS.hlsl", "main" );
	}
	else if( keyboard[ Keyboard::PhysicalKeys::KEY_4 ].IsKeyDownEvent() )
	{
		m_animVertexShader = m_resourceManager->LoadVertexShader( L"Shaders/AnimationRaycasterGPU/AnimLoadedNormalsVS.hlsl", "main" );
		m_animGeometryShader = nullptr;
		m_animPixelShader = m_resourceManager->LoadPixelShader( L"Shaders/AnimationRaycasterGPU/AnimLoadedNormalsPS.hlsl", "main" );
	}
	else if( keyboard[ Keyboard::PhysicalKeys::KEY_5 ].IsKeyDownEvent() )
	{
		m_animVertexShader = m_resourceManager->LoadVertexShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimation3VS.hlsl", "main" );
		m_animGeometryShader = m_resourceManager->LoadGeometryShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimation3GS.hlsl", "main" );
		m_animPixelShader = m_resourceManager->LoadPixelShader( L"Shaders/AnimationRaycasterGPU/VoxelAnimation3PS.hlsl", "main" );
	}

	if( keyboard[ Keyboard::PhysicalKeys::KEY_NUMPADPLUS ].IsPressed() )
	{
		m_offsetShell += 0.0001f;
	}
	else if( keyboard[ Keyboard::PhysicalKeys::KEY_NUMPADMINUS ].IsPressed() )
	{
		m_offsetShell -= 0.0001f;
	}

	if( keyboard[ Keyboard::PhysicalKeys::KEY_NUMPAD8 ].IsPressed() )
	{
		m_maxError += 0.0001f;
	}
	else if( keyboard[ Keyboard::PhysicalKeys::KEY_NUMPAD2 ].IsPressed() )
	{
		m_maxError -= 0.0001f;
	}

	if( keyboard[ Keyboard::PhysicalKeys::KEY_TAB ].IsKeyDownEvent() )
	{
		m_enableShellDisplay = !m_enableShellDisplay;
	}
}


//====================================================================================//
//			Data transfer to GPU	
//====================================================================================//


// ================================ //
//
void				AnimationRaycasterGPU::UpdateCamera		( CameraActor* camera )
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

// ================================ //
//
void				AnimationRaycasterGPU::UpdateAnimation			( TimeType time, ShellMeshPtr shellMesh )
{
	auto & bonesTransforms = shellMesh->Evaluate( time );

	if( bonesTransforms.size() > 200 )
		throw std::runtime_error( "Number of bones exceeds max value." );

	uint32 bufferSize = sizeof( Transform ) * (uint32)bonesTransforms.size();

	if( !m_bonesBuffer )
	{
		m_bonesBuffer = m_resourceManager->CreateConstantsBuffer( L"BonesTransforms", (uint8*)bonesTransforms.data(), bufferSize );
		return;
	}

	if( m_bonesBuffer->GetElementSize() != bufferSize )
	{
		// @todo Here we should free previous buffer.
		m_bonesBuffer = m_resourceManager->CreateConstantsBuffer( L"BonesTransforms", (uint8*)bonesTransforms.data(), bufferSize );
		return;
	}

	UpdateBufferCommand updateCommand;
	updateCommand.Buffer = m_bonesBuffer.Ptr();
	updateCommand.FillData = (uint8*)bonesTransforms.data();
	updateCommand.Size = bufferSize;

	m_renderer->UpdateBuffer( updateCommand );
}

// ================================ //
//
void				AnimationRaycasterGPU::UpdateMeshBuffer			( ShellMeshPtr shellMesh )
{
	MeshContants meshConstants;
	meshConstants.Translate = shellMesh->GetTranslate();
	meshConstants.Scale = shellMesh->GetScale();
	meshConstants.OffsetShell = m_offsetShell;
	meshConstants.EnableShellDisplay = m_enableShellDisplay;
	meshConstants.MaxError = m_maxError;

	uint32 bufferSize = sizeof( MeshContants );

	if( !m_meshTransformBuffer )
	{
		m_meshTransformBuffer = m_resourceManager->CreateConstantsBuffer( L"ShellMeshTransforms", (uint8*)&meshConstants, bufferSize );
		return;
	}

	UpdateBufferCommand updateCommand;
	updateCommand.Buffer = m_meshTransformBuffer.Ptr();
	updateCommand.FillData = (uint8*)&meshConstants;
	updateCommand.Size = bufferSize;

	m_renderer->UpdateBuffer( updateCommand );
}

// ================================ //
//
void				AnimationRaycasterGPU::ReallocateRenderTarget	( uint16 newWidth, uint16 newHeight )
{
	// Allocate render target for shell mesh rendering result.
	RenderTargetDescriptor descriptor;
	descriptor.TextureWidth = newWidth;
	descriptor.TextureHeight = newHeight;
	descriptor.TextureType = TextureType::TEXTURE_TYPE_TEXTURE2D;
	descriptor.ColorBuffFormat = ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_UNORM;
	descriptor.DepthStencilFormat = DepthStencilFormat::DEPTH_STENCIL_FORMAT_D16_UNORM;
	descriptor.Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;

	m_shellMeshTarget = m_resourceManager->CreateRenderTarget( L"::ShellMeshTarget", descriptor );
	assert( m_shellMeshTarget );
}



}	// vr


