#pragma once

#include "swCommonLib/Common/EngineObject.h"
#include "swGraphicAPI/Rendering/IGraphicAPIInitializer.h"
#include "swGUI/Native/INativeGUI.h"
#include "Events/EventsSystem.h"
#include "swGUI/Core/System/Rendering/RenderingSystem.h"

#include "HostWindow.h"

class ResourceManager;


namespace sw {
namespace gui
{


/**@defgroup SWGUI Sleeping Wombat GUI
@ingroup ModulesStructure
@brief Graphic user interface library based on GPU rendering.


@mainpage

@section Intro Introduction

This GUI is platform independent system for writing GUI applications in C++.
The main idea of this gui is to cover the most common WPF functionalities and allow user to load already created xaml
sheets. Of course WPF is to rich to implement all features.
The key features are:
- Support for data binding (gui uses RTTR reflection system)
- Separating controls functionality and visual behavior (control templates, styles)
- Extensibility and controls customization
- GPU rendering

GUI will support similar controls with similar properties, but we will choose only most usefull properties which 
we used in our short practise with WPF, so porting from WPF to Wombat GUI will not always be simple.

The second goal of this gui are game engines. GUI system should be efficient enough to be used in real time graphics
and to not consume to much CPU and GPU resources. These two goals can be defficult to bring together...


Sleeping Wombat GUI is still in development process.

@section RoadMap Road map

1. Handling main input events (keyboard mouse)
2. Hit testing, mouse and keyboard capture
3. Basic rendering - rectangle, circle with using simple brushes
4. Controls layout and arrangment system
5. Controls visual states (Fake implementation for testing)
6. Basic controls (Buttons, StackPanels ...)
7. Data binding
8. Control Templates
9. Storyboard and visual states - proper implementation
10. XAML deserialization
11. Control Styles
12. Basic controls set (Buttons, Layout controls, Tree, List, Window)
13. ...




@section Installation

Sleeping Wombat gui uses:
- https://github.com/nieznanysprawiciel/swInputLibrary
- https://github.com/nieznanysprawiciel/swCommonLib
- https://github.com/nieznanysprawiciel/swGraphicAPI

The best option is to add all libraries as subtrees to your project. Imagine that you have all libraries in folder /External.
Your directory should look like this:

- External/
	- swInputLibrary/
	- swGraphicAPI/
	- swGUI/

You must add folder External to additional include directories. All paths in sw libraries are relative to folder ../

@subsection Installation_Linking Linking Libraries

The best way to link sw libraries is to add all projects to your own solution (for example make merge solution).
Then you can simply add CoreGUI as reference. Then Add swGUI\ProjectDir\Visual2015\LinkSleepingWombatGUI.props file
to project that uses GUI.


Note that all libraries are in development and not all commits will work together.
In future we will tag which versions should be used or think of better solution for that, but first all
libraries must stabilize.

@subsection Installation_Usage

Usage example of GUI:
main.cpp

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

Note you must reference specific Native API in project.

Application.h
@code
#pragma once

#include "swGUI/Core/System/GUISystem.h"



class Application : public sw::gui::GUISystem
{
private:
protected:
public:
	explicit	Application		( int argc, char** argv, sw::gui::INativeGUI* gui );
				~Application	() = default;

protected:
	virtual	void	Initialize		() override;
	virtual void	OnInitialized	() override;
	virtual void	OnClosing		() override;
	virtual void	OnIdle			() override;

};

@endcode


Application.cpp

@code

#include "Application.h"


#include "Sizeofs/Sizeofs.h"



// ================================ //
//
Application::Application	( int argc, char** argv, sw::gui::INativeGUI* gui )
	:	sw::gui::GUISystem( argc, argv, gui )
{}

// ================================ //
//
void Application::Initialize()
{
	DefaultInit( 1024, 768, "Window Tittle" );
}

// ================================ //
//
void		Application::OnInitialized()
{
	// In this function sizeofs basic classes are printed. Test purposes only.
	PrintSizeofs();

	CreateNativeHostWindow( 500, 500, "Additional window" );
}

// ================================ //
//
void		Application::OnClosing()
{ }

// ================================ //
//
void		Application::OnIdle()
{ }

@endcode



In future we will provide better usage examples!

*/


/**@defgroup GUICore Core
@ingroup SWGUI
@brief Core modules and functionalities..*/


/**@brief Structure for command line arguments.

@todo Move to separate file.*/
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

@ingroup GUICore

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
	friend class GUISystemTester;
private:

	static GUISystem*			m_instance;

protected:

	IGraphicAPIInitializer*		m_graphicApi;		///< Contains object responsible for dealing with specifics graphic apis.
	INativeGUI*					m_nativeGUI;		///< Native window system used to display main application window.
	input::IInput*				m_input;			///< Input processor.

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
	bool			MainLoopCore	();
	void			HandleEvents	();
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
