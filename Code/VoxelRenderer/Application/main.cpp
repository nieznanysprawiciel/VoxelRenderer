#include "swGUI/Core/System/GUISystem.h"
#include "swGUI/Native/WinAPI/WinAPIGUI.h"

#include "Application.h"


int main( int argc, char** argv )
{
	Application app( argc, argv, GUI::WinAPIGUI::Create() );
	app.Init();

	return app.MainLoop();
}

