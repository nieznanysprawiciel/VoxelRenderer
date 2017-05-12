#include "Application.h"


#include "Sizeofs/Sizeofs.h"

#include <iostream>


using namespace sw::gui;

// ================================ //
//
void		MouseMoveEventReceived			( UIElement* sender, MouseMoveEventArgs* e )
{
	std::cout	<< "MouseMove Window [" << sender->GetHost()->GetNativeWindow()->GetTitle()
				<< "] Position [" << e->WindowPosX << ", " << e->WindowPosY << "], Delta [" 
				<< e->MouseDeltaX << ", " << e->MouseDeltaY << "]" << std::endl;
}


// ================================ //
//
Application::Application	( int argc, char** argv, sw::gui::INativeGUI* gui )
	:	sw::gui::GUISystem( argc, argv, gui )
{}


/**@brief GUI subsystems initialization.

If you need specific gui initialization in your application override this function.
You can set different GraphicApi or input api.*/
void Application::Initialize()
{
	DefaultInit( 1024, 768, "Window Tittle" );
}

/**@brief Function is called when GUI initialization is completed.

In this function you should initialize your application logic.
*/
void		Application::OnInitialized()
{
	// In this function sizeofs basic classes are printed. Test purposes only.
	PrintSizeofs();

	HostWindow* window = CreateNativeHostWindow( 500, 500, "Additional window" );
	window->PreviewMouseMove() += MouseMoveEventHandler( &MouseMoveEventReceived );
	m_windows[ 0 ]->PreviewMouseMove() += MouseMoveEventHandler( &MouseMoveEventReceived );
}

/**@brief Function invoked when application is going to close itself.*/
void		Application::OnClosing()
{ }

/**@brief */
void		Application::OnIdle()
{ }
