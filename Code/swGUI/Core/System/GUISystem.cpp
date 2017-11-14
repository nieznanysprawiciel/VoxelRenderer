#include "GUISystem.h"

#include "swInputLibrary/InputCore/KeyboardState.h"
#include "swGraphicAPI/Resources/ResourcesFactory.h"
#include "swGraphicAPI/ResourceManager/ResourceManager.h"

#include <map>
#include <string>



namespace sw {
namespace gui
{

/// @todo This should be moved domewhere else.
INativeGUI*			CreateNativeGUI();



GUISystem*			GUISystem::m_instance = nullptr;


// ================================ //
//
GUISystem::GUISystem( int argc, char** argv, INativeGUI* gui )
	:	m_cmdArgs( argc, argv )
	,	m_nativeGUI( gui )
	,	m_focusedWindow( nullptr )
	,	m_resourceManager( nullptr )
	,	m_input( nullptr )
	,	m_renderingSystem( nullptr )
{
	m_instance = this;
}

// ================================ //
//
GUISystem::~GUISystem()
{
	for( auto window : m_windows )
		delete window;

	delete m_resourceManager;
	m_graphicApi->ReleaseAPI();
	delete m_graphicApi;
	delete m_nativeGUI;
}

//====================================================================================//
//			Main loop	
//====================================================================================//


/**@brief Application main loop.

@see Application*/
int					GUISystem::MainLoop()
{
	while( true )
	{
		bool end = MainLoopCore();
		if( end ) break;
	}

	CloseLogic();

	return 0;
}

/**@brief One step of main loop.
@todo Consider making this function virtual in future.

@return Returns true if application should end. Otherwise returns false.*/
bool				GUISystem::MainLoopCore()
{
	// Process native events.
	bool end = m_nativeGUI->MainLoop( m_guiConfig.UseBlockingMode );
	if( end ) return true;

	auto frameTime = m_clock.AdvanceFrame();

	HandleEvents( frameTime );

	// @todo How should it be done ??
	OnIdle( frameTime );
	RenderGUI( frameTime );

	return false;
}

/**@brief Processes messages and passes them to focused window.*/
void				GUISystem::HandleEvents		( const FrameTime& frameTime )
{
	// @todo We should pass correct time in parameter.
	m_input->Update( (float)frameTime.Elapsed );

	if( m_focusedWindow )
		m_focusedWindow->HandleInput();
	else
	{
		for( auto& device : m_input->GetKeyboardDevice() )
			device->ApplyAllEvents();

		for( auto& device : m_input->GetMouseDevice() )
			device->ApplyAllEvents();

		for( auto& device : m_input->GetJoystickDevice() )
			device->ApplyAllEvents();
	}
}

// ================================ //
//
void				GUISystem::RenderGUI		( const FrameTime& frameTime )
{
	if( m_guiConfig.RedrawOnlyFocused && m_focusedWindow )
		m_focusedWindow->GetSwapChain()->Present( GetSyncInterval() );

	if( !m_guiConfig.RedrawOnlyFocused )
	{
		for( auto window : m_windows )
			window->GetSwapChain()->Present( GetSyncInterval() );
	}
}

// ================================ //
//
void				GUISystem::CloseLogic		()
{
	OnClosing();
}


//====================================================================================//
//			Initialization	
//====================================================================================//

/**@brief Invoke this function in application entry point (main).*/
bool				GUISystem::Init()
{
	bool result = true;

	result = result && Initialize();		// Initialize subsystems.
	result = result && OnInitialized();		// User initialization.

	m_clock.Start();	// Start clock as last in initialization.

	return result;
}


/**@brief GUI subsystems initialization.

If you need specific gui initialization in your application override this function.
You can set different GraphicApi or input api.*/
bool				GUISystem::Initialize()
{
	return DefaultInit( 1024, 768, "Window Tittle" );
}

/**@brief Makes initialization but leaves window creation for user.*/
bool				GUISystem::DefaultInitWithoutWindow	()
{
	bool result = true;

	result = result && DefaultInitResourceManager();
	result = result && DefaultInitGraphicAPI( false, true );
	result = result && DefaultInitNativeGUI();
	result = result && DefaultInitRenderingSystem();

	return result;
}

/**@brief Default GUI system initialization function.*/
bool				GUISystem::DefaultInit				( uint16 width, uint16 height, const std::string& windowTitle )
{
	bool result = true;

	result = result && DefaultInitWithoutWindow();
	result = result && DefaultInitFirstWindow( width, height, windowTitle, true );

	return result;
}


/**@brief Default native GUI initialization.

Native GUI object is set in constructor. Here we initialize it.
Function creates native input object.*/
bool				GUISystem::DefaultInitNativeGUI		()
{
	NativeGUIInitData nativeGUIInit;
	nativeGUIInit.FocusChanged = fastdelegate::MakeDelegate( this, &GUISystem::OnFocusChanged );

	bool result = m_nativeGUI->Init( nativeGUIInit );
	if( result )
	{
		m_input = m_nativeGUI->UseNativeInput();
		assert( m_input );
	}

	return result;
}

/**@brief Default graphic api initialization.*/
bool				GUISystem::DefaultInitGraphicAPI	( bool debug, bool singleThreaded )
{
	// ResourceFactory creates api which is linked as library.
	m_graphicApi = ResourcesFactory::CreateAPIInitializer();

	GraphicAPIInitData graphicApiData;
	graphicApiData.CreateSwapChain = false;		// We will create swap chain and render target later with window.
	graphicApiData.SingleThreaded = singleThreaded;
	graphicApiData.UseDebugLayer = true;

	auto result = m_graphicApi->InitAPI( graphicApiData );
	assert( result );

	return result;
}

/**@brief Initializes rendering system.

@note m_resourceManager and m_graphicApi must be already initialized;*/
bool				GUISystem::DefaultInitRenderingSystem	()
{
	if( !m_graphicApi )
		return false;

	if( !m_resourceManager )
		return false;

	IRendererOPtr renderer = std::unique_ptr< IRenderer >( m_graphicApi->CreateRenderer( RendererUsage::USE_AS_IMMEDIATE ) );
	m_renderingSystem = std::unique_ptr< RenderingSystem >( new RenderingSystem( m_resourceManager, std::move( renderer ) ) );

	return true;
}

// ================================ //
//
bool				GUISystem::DefaultInitFirstWindow		( uint16 width, uint16 height, const std::string& windowTitle, bool show )
{
	// Note: we must always initialize first focus window. This is probably hack, but OnFocusChanged delegate won't be invoked.
	m_focusedWindow = CreateNativeHostWindow( width, height, windowTitle );
	assert( m_focusedWindow );
	m_focusedWindow->GotFocus();

	if( show )
		m_focusedWindow->GetNativeWindow()->Show();
	else
		m_focusedWindow->GetNativeWindow()->Hide();

	return true;
}

/**@brief Function creates ResourceManager and calls default initialization.*/
bool				GUISystem::DefaultInitResourceManager	()
{
	m_resourceManager = new ResourceManager();
	return ResourceManagerInitImpl( m_resourceManager );
}

/**@brief This functions sets ResourceManager from parameter and calls default initialization.*/
bool				GUISystem::InitResourceManager			( ResourceManager* resourceManager )
{
	m_resourceManager = resourceManager;
	return ResourceManagerInitImpl( m_resourceManager );
}

// ================================ //
//
bool				GUISystem::ResourceManagerInitImpl		( ResourceManager* resourceManager )
{
	// Empty for future use
	return true;
}

//====================================================================================//
//			Focus management	
//====================================================================================//

/**@brief Changes focused window.

Delegate for native GUI.*/
void				GUISystem::OnFocusChanged				( INativeWindow* window, bool value )
{
	if( !value )
	{
		// Only lost focus
		if( m_focusedWindow && m_focusedWindow->GetNativeWindow() == window )
		{
			m_focusedWindow->LostFocus();
			m_focusedWindow = nullptr;
		}
		//else
		// ReportError
	}
	else
	{
		// Previous window loses focus.
		if( m_focusedWindow && m_focusedWindow->GetNativeWindow() == window )
		{
			return;
		}
		else if( m_focusedWindow && m_focusedWindow->GetNativeWindow() != window )
		{
			m_focusedWindow->LostFocus();
			m_focusedWindow = nullptr;
		}

		// Find new window with focus.
		for( auto hostWindow : m_windows )
		{
			if( hostWindow->GetNativeWindow() == window )
			{
				m_focusedWindow = hostWindow;
				m_focusedWindow->GotFocus();
				break;
			}
		}

		//if( !m_focusedWindow )
		// ReportError
	}
}

//====================================================================================//
//			Windows creation	
//====================================================================================//


// ================================ //
//
HostWindow*			GUISystem::CreateNativeHostWindow	( uint16 width, uint16 height, const std::string& windowTitle )
{
	NativeWindowDescriptor init( windowTitle );
	init.Height = height;
	init.Width = width;

	return CreateNativeHostWindow( init );
}

// ================================ //
//
HostWindow*			GUISystem::CreateNativeHostWindow	( NativeWindowDescriptor& windowDesc )
{
	auto nativeWindow = m_nativeGUI->CreateWindow( windowDesc );
	assert( nativeWindow );

	// Create host window from underlying native window.
	HostWindow* hostWindow = new HostWindow( nativeWindow, m_input, m_resourceManager, m_graphicApi );
	m_windows.push_back( hostWindow );

	return hostWindow;
}

//====================================================================================//
//				Other functions
//====================================================================================//

/**@copydoc EngineObject::MemorySize*/
Size				GUISystem::GetMemorySize()
{
	Size size = sizeof( HostWindow );
	size += m_windows.capacity() * sizeof( HostWindow* );

	for( auto window : m_windows )
		size += window->GetMemorySize();

	return size;
}

/**@brief Gets number of commnad line arguments.

Function doesn't take program nameinto account .*/
int					GUISystem::NumCommandLineArgs()
{
	return m_cmdArgs.ArgsCount - 1;
}

/**@brief Returns command line argument. 0 is first argument
not program name.*/
const char*			GUISystem::CommandLineArg( int num )
{
	return m_cmdArgs.Arguments[ num - 1 ];
}

/**@brief Returns program name retrived from 0 command line argument.*/
const char*			GUISystem::ProgramPath()
{
	return m_cmdArgs.ProgramName();
}


//====================================================================================//
//			Internal implementation	
//====================================================================================//

// ================================ //
//
int					GUISystem::GetSyncInterval() const
{
	return m_guiConfig.UseVSync ? 1 : 0;
}

/**@brief */
GUISystem&			GUISystem::Get()
{
	return *m_instance;
}


}	// gui
}	// sw

