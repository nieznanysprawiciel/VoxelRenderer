#pragma once

#include "swCommonLib/Common/EngineObject.h"
#include "swGraphicAPI/Rendering/IGraphicAPIInitializer.h"
#include "swGUI/Native/INativeGUI.h"
#include "Events/EventsManager.h"
#include "swGUI/Core/System/Rendering/RenderingSystem.h"

#include "HostWindow.h"

class ResourceManager;


namespace sw {
namespace gui
{


struct CommandLineArgs
{
	int				ArgsCount;
	char**			Arguments;


	CommandLineArgs( int argc, char** argv )
		: ArgsCount( argc )
		, Arguments( argv )
	{ }

	const char*		ProgramName	() { return Arguments[ 0 ]; }
};


/**@brief Main GUI application class.

@ingroup EngineGUI

Use example of GUI:
@code
#include "swGUI/Core/System/GUISystem.h"
#include "swGUI/Native/WinAPI/WinAPIGUI.h"

#include "Application.h"

int main( int argc, char** argv )
{
	Application app( argc, argv, GUI::WinAPIGUI::Create() );
	app.Init();

	return app.MainLoop();
}
@endcode

You don't have to write main function. Instead use project templates for different platforms.
To use gui implement Application class.

@todo Supply project templates.
*/
class GUISystem
{
private:

	static GUISystem*			m_instance;

protected:

	IGraphicAPIInitializer*		m_graphicApi;		///< Contains object responsible for dealing with specifics graphic apis.
	INativeGUI*					m_nativeGUI;		///< Native window system used to display main application window.
	IInput*						m_input;			///< Input processor.

	ResourceManager*			m_resourceManager;	///< Resources.
	RenderingSystemOPtr			m_renderingSystem;	///< All rendering connceted functionalities.

	std::vector< HostWindow* >	m_windows;
	HostWindow*					m_focusedWindow;	///< Only one window can have focus and only this will be processed.

	CommandLineArgs				m_cmdArgs;

public:
	explicit		GUISystem		( int argc, char** argv, INativeGUI* gui );
	virtual			~GUISystem		();

	Size			GetMemorySize	();

	int				NumCommandLineArgs	();
	const char*		CommandLineArg		( int num );
	const char*		ProgramPath			();

protected:
	///@name User overrides
	///@{
	virtual	void	Initialize		();
	virtual void	OnInitialized	() = 0;
	virtual void	OnClosing		() = 0;
	virtual void	OnIdle			() = 0;
	///@}

	///@name Default initialization functions
	///@{
	void			DefaultInitWithoutWindow	();
	void			DefaultInit					( uint16 width, uint16 height, const std::string& windowTitle );
	bool			DefaultInitNativeGUI		();
	bool			DefaultInitGraphicAPI		();
	bool			DefaultInitRenderingSystem	();
	///@}

public:

	///@name Main functions
	///@{
	void			Init			();
	int				MainLoop		();
	///@}

public:

	void			OnFocusChanged				( INativeWindow* window, bool value );
	void			OnResized					( INativeWindow* window, uint16 newWidth, uint16 newHeight );
	void			OnMaximized					( INativeWindow* window );
	void			OnMinimized					( INativeWindow* window );

	/**@brief Calls CreateNativeHostWindow with default values.
	@see GUISystem::CreateNativeHostWindow*/
	HostWindow*		CreateNativeHostWindow		( uint16 width, uint16 height, const std::string& windowTitle );
	/**@brief Creates host window based on native window.
	Window will be added to windows list.
	@note windowDesc can change. For instance: if you set AdjustSize to true, function will return real window size.*/
	HostWindow*		CreateNativeHostWindow		( NativeWindowDescriptor& windowDesc );


public:
	static GUISystem&	Get				();
};


}	// gui
}	// sw
