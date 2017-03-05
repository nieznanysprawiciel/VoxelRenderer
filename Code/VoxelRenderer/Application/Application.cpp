#include "Application.h"

#include "VoxelRenderer/Raycaster/RaycasterCPU.h"



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
	//m_raycaster->Render( m_octree, , m_camera );


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
	else
	{
		/// Error !
	}
/*
	if( m_raycaster )
		m_raycaster->Init( )*/
}

// ================================ //
//
void		Application::InitOctree		()
{


}


}	// vr
