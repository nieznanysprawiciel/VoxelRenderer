#include "swGUI/Core/System/GUISystem.h"
#include "swGUI/Native/WinAPI/WinAPIGUI.h"

#include "Application.h"


int main( int argc, char** argv )
{
	vr::Application app( argc, argv, sw::gui::WinAPIGUI::Create() );
	app.Init();

	return app.MainLoop();
}

