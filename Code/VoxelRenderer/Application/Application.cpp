#include "Application.h"


namespace vr
{


// ================================ //
//
Application::Application	( int argc, char** argv, sw::gui::INativeGUI* gui )
	: sw::gui::GUISystem( argc, argv, gui )
{}


/**@brief GUI subsystems initialization.

If you need specific gui initialization in your application override this function.
You can set different GraphicApi or input api.*/
void Application::Initialize()
{
	DefaultInit( 1024, 768, "Voxel skeletal animation" );
}

/**@brief Function is called when GUI initialization is completed.

In this function you should initialize your application logic.
*/
void		Application::OnInitialized()
{}

/**@brief Function invoked when application is going to close itself.*/
void		Application::OnClosing()
{}

/**@brief */
void		Application::OnIdle()
{}


}	// vr
