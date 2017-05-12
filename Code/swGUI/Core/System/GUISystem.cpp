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

	m_graphicApi->ReleaseAPI();
	delete m_resourceManager;
	delete m_graphicApi;
	delete m_nativeGUI;
}


/**@brief Application main loop.

@see Application*/
int					GUISystem::MainLoop()
{
	while( true )
	{
		bool end = MainLoopCore();
		if( end ) break;
	}

	OnClosing();

	return 0;
}

/**@brief One step of main loop.
@todo Consider making this function virtual in future.

@return Returns true if application should end. Otherwise returns false.*/
bool				GUISystem::MainLoopCore()
{
	// Process native events.
	bool end = m_nativeGUI->MainLoop( true );
	if( end ) return true;;

	HandleEvents();

	// @todo How should it be done ??
	OnIdle();
	if( m_focusedWindow )
		m_focusedWindow->GetSwapChain()->Present( 1 );

	return false;
}

/**@brief Processes messages and passes them to focused window.*/
void				GUISystem::HandleEvents()
{
	// @todo We should pass correct time in parameter.
	m_input->Update( 0.0 );

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


/**@brief Invoke this function in application entry point (main).*/
void				GUISystem::Init()
{
	Initialize();		// Initialize subsystems.
	OnInitialized();	// User initialization.
}


/**@brief GUI subsystems initialization.

If you need specific gui initialization in your application override this function.
You can set different GraphicApi or input api.*/
void				GUISystem::Initialize()
{
	DefaultInit( 1024, 768, "Window Tittle" );
}

/**@brief Makes initialization but leaves window creation for user.*/
void				GUISystem::DefaultInitWithoutWindow	()
{
	m_resourceManager = new ResourceManager();

	DefaultInitGraphicAPI();
	DefaultInitNativeGUI();
	DefaultInitRenderingSystem();
}

/**@brief Default GUI system initialization function.*/
void				GUISystem::DefaultInit				( uint16 width, uint16 height, const std::string& windowTitle )
{
	DefaultInitWithoutWindow();

	// Note: we must always initialize first focus window. This is probably hack, but OnFocusChanged delegate won't be invoked.
	m_focusedWindow = CreateNativeHostWindow( width, height, windowTitle );
	assert( m_focusedWindow );
	m_focusedWindow->GotFocus();
}


/**@brief Default native GUI initialization.

Native GUI object is set in constructor. Here we initialize it.
Function creates native input object.*/
bool				GUISystem::DefaultInitNativeGUI		()
{
	NativeGUIInitData nativeGUIInit;
	nativeGUIInit.FocusChanged = fastdelegate::MakeDelegate( this, &GUISystem::OnFocusChanged );

	m_nativeGUI->Init( nativeGUIInit );

	m_input = m_nativeGUI->UseNativeInput();
	assert( m_input );

	return true;
}

/**@brief Default graphic api initialization.*/
bool				GUISystem::DefaultInitGraphicAPI	()
{
	// ResourceFactory creates api which is linked as library.
	m_graphicApi = ResourcesFactory::CreateAPIInitializer();
	GraphicAPIInitData graphicApiData;
	graphicApiData.CreateSwapChain = false;		// We will create swap chain and render target later with window.

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

/**@brief Changes focused window.

Delegate for native GUI.*/
void				GUISystem::OnFocusChanged	( INativeWindow* window, bool value )
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



/**@brief */
GUISystem&			GUISystem::Get()
{
	return *m_instance;
}


}	// gui
}	// sw

