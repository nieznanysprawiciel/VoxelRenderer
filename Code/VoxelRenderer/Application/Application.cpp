#include "Application.h"

#include "VoxelRenderer/Raycaster/RaycasterCPU.h"
#include "VoxelRenderer/Raycaster/FakeRaycaster.h"

#include "VoxelRenderer/Actors/CameraController.h"

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
	InitResources();
	InitCamera();
	InitRaycaster();
	InitOctree();
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
{
	m_camera->GetController()->ControlObjectPre( m_camera, nullptr );
	m_camera->GetController()->ControlObjectPost( m_camera, nullptr );
}


// ================================ //
//
void		Application::Render()
{
	m_timeManager.onStartRenderFrame();
	double time = m_timeManager.QueryTimeFromBegin();
		

	m_raycaster->Render( m_octree, m_svoRT.Ptr(), m_camera );
	//m_raycaster->Render( m_octree, m_mainRT.Ptr(), m_camera );
	m_blitEffect->Blit( m_renderingSystem->GetRenderer(), m_svoRT->GetColorBuffer(), m_mainRT.Ptr() );
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
{
	m_octree = std::make_shared< Octree >();

	filesystem::Path filePath = m_config->OctreeFilePath();
	if( filePath.Exists() )
	{
		bool result = m_octree->LoadFromFile( filePath );
		assert( result );
	}
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



}	// vr
