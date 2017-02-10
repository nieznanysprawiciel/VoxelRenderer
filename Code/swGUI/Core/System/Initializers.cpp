#include "swGUI/Native/INativeGUI.h"



#define USE_WIN32_API

#ifdef USE_WIN32_API
	#include "swGUI/Native/WinAPI/WinAPIGUI.h"
	typedef GUI::WinAPIGUI NativeGUI;
#else
	#error Macro USE_WIN32_API not defined!
#endif



namespace GUI
{



INativeGUI*			CreateNativeGUI()
{
	return new NativeGUI();
}


}	// GUI
