#include "Application.h"

#include "VoxelRenderer/Raycaster/RaycasterCPU.h"
#include "VoxelRenderer/Raycaster/RaycasterGPU.h"
#include "VoxelRenderer/Raycaster/FakeRaycaster.h"
#include "VoxelRenderer/Raycaster/PrintOctreeRaycaster.h"
#include "VoxelRenderer/Raycaster/DepthRaycaster.h"
#include "VoxelRenderer/Raycaster/NormalsRaycaster.h"
#include "VoxelRenderer/Raycaster/ShellMeshRenderer.h"
#include "VoxelRenderer/Raycaster/AnimationRaycasterGPU.h"


#include "VoxelRenderer/Actors/CameraController.h"

#include "VoxelRenderer/ShellMesh/FBXLoader.h"

namespace vr
{


// ================================ //
//
Application::Application	( int argc, char** argv, sw::gui::INativeGUI* gui )
	:	sw::gui::GUISystem( argc, argv, gui )
	,	m_camera( new CameraActor())
	,	m_blitEffect( nullptr )
{}


/**@brief GUI subsystems initialization.

If you need specific gui initialization in your application override this function.
You can set different GraphicApi or input api.*/
bool		Application::Initialize			()
{
	m_config = MakeUPtr< Config >( "StartConfig.config" );
	return DefaultInit( m_config->ScreenWidth(), m_config->ScreenHeight(), "Voxel skeletal animation" );
}

/**@brief Function is called when GUI initialization is completed.

In this function you should initialize your application logic.
*/
bool		Application::OnInitialized		()
{
	InitLights();
	InitResources();
	InitCamera();
	InitRaycaster();
	InitOctree();
	InitShellMesh();

	m_guiConfig.UseBlockingMode = false;
	m_guiConfig.UseVSync = m_config->EnableVSync();

	return true;
}

/**@brief Function invoked when application is going to close itself.*/
void		Application::OnClosing			()
{}

/**@brief */
void		Application::OnIdle				( const sw::gui::FrameTime& frameTime )
{
	Update( frameTime );
	Render( frameTime );
}


//====================================================================================//
//				
//====================================================================================//

// ================================ //
//
void		Application::Update				( const sw::gui::FrameTime& frameTime )
{
	UpdateTime( frameTime );

	// Camera should move even when everything is paused. Pass real frame time.
	m_camera->GetController()->ControlObjectPre( m_camera, nullptr, frameTime.Time, frameTime.Elapsed );
	m_camera->GetController()->ControlObjectPost( m_camera, nullptr, frameTime.Time, frameTime.Elapsed );

	auto animFrameTime = m_timeManager.GetCurrentFrameTime();

	m_lightModule->Update( animFrameTime.Time, animFrameTime.Elapsed );
}


// ================================ //
//
void		Application::UpdateTime		( const sw::gui::FrameTime & frameTime )
{
	m_timeManager.ProcessInput( m_input->GetMouseDevice()[ 0 ]->GetState(), m_input->GetKeyboardDevice()[ 0 ]->GetState() );
	m_timeManager.ProcessTime( frameTime );

	if( m_timeManager.GetFPSCounter().NeedsFPSRefresh() )
	{
		auto fpsText = m_timeManager.GetFPSCounter().PrintFPS();
		m_windows[ 0 ]->GetNativeWindow()->SetTitle( "Voxel skeletal animation. " + fpsText );
	}
}

// ================================ //
//
void		Application::Render				( const sw::gui::FrameTime& frameTime )
{
	auto animFrameTime = m_timeManager.GetCurrentFrameTime();

	m_raycaster->ProcessInput( m_input->GetMouseDevice()[ 0 ]->GetState(), m_input->GetKeyboardDevice()[ 0 ]->GetState() );

	m_raycaster->RenderShellMeshes( (vr::TimeType)animFrameTime.Time, m_shellMeshes, m_camera );
	m_raycaster->Render( (vr::TimeType)animFrameTime.Time, m_octree, m_svoRT.Ptr(), m_camera );

	m_blitEffect->Blit( m_renderingSystem->GetRenderer(), m_svoRT->GetColorBuffer(), m_mainRT.Ptr() );
}

// ================================ //
//
void		Application::InitLights		()
{
	m_lightModule = std::make_shared< LightModule >( *m_config.get() );
}

// ================================ //
//
void		Application::InitCamera		()
{
	m_camera->SetPerspective( m_config->IsPerspective() );
	m_camera->SetWidth( m_config->ScreenWidth() );
	m_camera->SetHeight( m_config->ScreenHeight() );
	m_camera->SetViewportSize( m_config->ViewportSize() );
	m_camera->SetFov( m_config->CameraFov() );
	m_camera->SetNearPlane( m_config->CameraNear() );
	m_camera->SetFarPlane( m_config->CameraFar() );
	m_camera->Teleport( DirectX::XMLoadFloat3( &m_config->CameraPosition() ) );


	DirectX::XMVECTOR defaultOrient = DirectX::XMVectorSet( 0.0, 0.0, -1.0, 0.0 );
	DirectX::XMVECTOR direction = DirectX::XMLoadFloat3( &m_config->CameraDirection() );
	direction = DirectX::XMVector3Normalize( direction );

	DirectX::XMVECTOR rotationAxis = DirectX::XMVector3Normalize( DirectX::XMVector3Cross( defaultOrient, direction ) );
	DirectX::XMVECTOR angle = DirectX::XMVector3AngleBetweenNormals( defaultOrient, direction );

	DirectX::XMVECTOR newOrientation = DirectX::XMQuaternionNormalize( DirectX::XMQuaternionRotationNormal( rotationAxis, DirectX::XMVectorGetX( angle ) ) );

	m_camera->TeleportOrientation( newOrientation );


	SpectatorCameraController* controller = new SpectatorCameraController( m_input->GetMouseDevice()[ 0 ]->GetState(), m_input->GetKeyboardDevice()[ 0 ]->GetState() );
	m_camera->SetController( controller );
}

// ================================ //
//
void		Application::InitRaycaster	()
{
	std::string raycasterType = m_config->RaycasterType();
	if( raycasterType == "CPU Raycaster" )
		m_raycaster = MakeUPtr< RaycasterCPU >();
	else if( raycasterType == "GPU Raycaster" )
		m_raycaster = MakeUPtr< RaycasterGPU >();
	else if( raycasterType == "FakeRaycaster" )
		m_raycaster = MakeUPtr< FakeRaycaster >();
	else if( raycasterType == "PrintOctree" )
		m_raycaster = MakeUPtr< PrintOctreeRaycaster >();
	else if( raycasterType == "DepthRaycaster" )
		m_raycaster = MakeUPtr< DepthRaycaster >();
	else if( raycasterType == "NormalsRaycaster" )
		m_raycaster = MakeUPtr< NormalsRaycaster >();
	else if( raycasterType == "ShellMeshRenderer" )
		m_raycaster = MakeUPtr< ShellMeshRenderer >();
	else if( raycasterType == "AnimationRaycasterGPU" )
		m_raycaster = MakeUPtr< AnimationRaycasterGPU >();
	else
	{
		/// Error !
	}

	if( m_raycaster )
		m_raycaster->Init( m_renderingSystem->GetRenderer(), m_resourceManager, m_lightModule );
}

// ================================ //
//
void		Application::InitOctree		()
{
	m_octree = LoadOctree( m_config->OctreeFilePath() );
}

// ================================ //
//
void		Application::InitResources()
{
	sw::gui::HostWindow* window = m_windows[ 0 ];
	m_mainRT = window->GetRenderTarget();

	RenderTargetDescriptor svoRTDescriptor;
	svoRTDescriptor.TextureWidth = m_config->ScreenWidth();
	svoRTDescriptor.TextureHeight = m_config->ScreenHeight();
	svoRTDescriptor.TextureType = TextureType::TEXTURE_TYPE_TEXTURE2D;
	svoRTDescriptor.ColorBuffFormat = ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_UNORM;
	svoRTDescriptor.DepthStencilFormat = DepthStencilFormat::DEPTH_STENCIL_FORMAT_D16_UNORM;
	svoRTDescriptor.Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;

	m_svoRT = m_resourceManager->CreateRenderTarget( L"::SVO_RenderTarget", svoRTDescriptor );
	assert( m_svoRT );

	auto blendingState = m_resourceManager->CreateBlendingState( L"::DefaultBlendingState", BlendingInfo() );
	auto rasterizerState = m_resourceManager->CreateRasterizerState( L"::DefaultRasterizerState", RasterizerStateInfo() );
	auto depthStencilState = m_resourceManager->CreateDepthStencilState( L"::DefaultDepthStencilState", DepthStencilInfo() );

	assert( blendingState );
	assert( rasterizerState );
	assert( depthStencilState );

	m_blitEffect = std::unique_ptr< BlitEffect >( new BlitEffect( m_resourceManager ) );
}

// ================================ //
//
void		Application::InitShellMesh()
{
	FBXLoader* loader = new FBXLoader();

	if( loader->CanLoad( m_config->ShellMeshFilePath() ) )
	{
		auto mesh = loader->LoadMesh( m_resourceManager, m_config->ShellMeshFilePath() );
		if( mesh.IsValid )
		{
			m_shellMeshes.push_back( mesh.Value );

			auto& animOctreeFile = m_config->AnimatedOctreePath();
			OctreePtr animOctree = LoadOctree( animOctreeFile );

			if( animOctree )
				mesh.Value->ApplyOctree( m_resourceManager, animOctree );
		}
	}

	delete loader;
}

// ================================ //
//
OctreePtr	Application::LoadOctree		( const std::string& octreeFile )
{
	OctreePtr octree = std::make_shared< Octree >();

	filesystem::Path filePath = octreeFile;
	if( filePath.Exists() )
	{
		bool result = octree->LoadFromFile( filePath );
		assert( result );

		return octree;
	}

	return nullptr;
}



}	// vr
