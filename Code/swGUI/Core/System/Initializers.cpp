#include "swGUI/Native/INativeGUI.h"



#define USE_WIN32_API

#ifdef USE_WIN32_API
	#include "swGUI/Native/WinAPI/WinAPIGUI.h"
	typedef sw::gui::WinAPIGUI NativeGUI;
#else
	#error Macro USE_WIN32_API not defined!
#endif



namespace sw {
namespace gui
{



INativeGUI*			CreateNativeGUI()
{
	return new NativeGUI();
}


}	// gui
}	// sw
