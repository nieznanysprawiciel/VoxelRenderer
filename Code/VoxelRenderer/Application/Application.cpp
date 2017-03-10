#include "Application.h"

#include "VoxelRenderer/Raycaster/RaycasterCPU.h"
#include "VoxelRenderer/Raycaster/FakeRaycaster.h"



namespace vr
{


// ================================ //
//
Application::Application	( int argc, char** argv, sw::gui::INativeGUI* gui )
	:	sw::gui::GUISystem( argc, argv, gui )
	,	m_camera( new CameraActor())
{}


/**@brief GUI subsystems initialization.

If you need specific gui initialization in your application override this function.
You can set different GraphicApi or input api.*/
void		Application::Initialize()
{
	m_config = MakeUPtr< Config >( "StartConfig.config" );
	DefaultInit( m_config->ScreenWidth(), m_config->ScreenHeight(), "Voxel skeletal animation" );
}

/**@brief Function is called when GUI initialization is completed.

In this function you should initialize your application logic.
*/
void		Application::OnInitialized()
{
	InitCamera();
	InitRaycaster();
	InitOctree();
	InitResources();
}

/**@brief Function invoked when application is going to close itself.*/
void		Application::OnClosing()
{}

/**@brief */
void		Application::OnIdle()
{
	Update();
	Render();
}


//====================================================================================//
//				
//====================================================================================//

// ================================ //
//
void		Application::Update()
{}


// ================================ //
//
void		Application::Render()
{
	m_timeManager.onStartRenderFrame();
	double time = m_timeManager.QueryTimeFromBegin();
		

	//m_raycaster->Render( m_octree, m_svoRT.Ptr(), m_camera );
	m_raycaster->Render( m_octree, m_mainRT.Ptr(), m_camera );
	//Blit( m_svoRT.Ptr(), m_mainRT.Ptr() );
}

// ================================ //
//
void		Application::InitCamera		()
{
	m_camera->SetFov( m_config->CameraFov() );
	m_camera->SetNearPlane( m_config->CameraNear() );
	m_camera->SetFarPlane( m_config->CameraFar() );
	m_camera->Teleport( DirectX::XMLoadFloat3( &m_config->CameraPosition() ) );

	//m_camera->SetDirection( DirectX::XMLoadFloat3( &m_config->CameraDirection() ) );
}

// ================================ //
//
void		Application::InitRaycaster	()
{
	std::string raycasterType = m_config->RaycasterType();
	if( raycasterType == "CPU Raycaster" )
		m_raycaster = MakeUPtr< RaycasterCPU >();
	else if( raycasterType == "FakeRaycaster" )
		m_raycaster = MakeUPtr< FakeRaycaster >();
	else
	{
		/// Error !
	}

	if( m_raycaster )
		m_raycaster->Init( m_renderingSystem->GetRenderer(), m_resourceManager );
}

// ================================ //
//
void		Application::InitOctree		()
{}

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
}

// ================================ //
//
void		Application::Blit			( RenderTargetObject* svoRenderTarget, RenderTargetObject* mainRT )
{

}


}	// vr
