#pragma once


#include "swGUI/Native/INativeGUI.h"

#include <Windows.h>
#undef CreateWindow
#undef XBUTTON1
#undef XBUTTON2



//struct tagMSG;
//typedef tagMSG MSG;
//
//struct HWND__;
//typedef HWND__* HWND;
//
//typedef UINT_PTR            WPARAM;
//typedef LONG_PTR            LPARAM;



namespace sw {
namespace input
{
class WinApiInputProxy;

}}	// sw::input




/**@defgroup Win32APINativeGUI WinAPI native GUI
@brief Native GUI basedon windows API.
@ingroup NativeGUI*/


namespace sw {
namespace gui
{


/**@brief 
@ingroup Win32APINativeGUI*/
class WinAPIGUI : public INativeGUI
{
private:
	sw::input::WinApiInputProxy*	m_input;
	NativeGUIInitData				m_initData;

protected:
public:
	explicit			WinAPIGUI	();
	virtual				~WinAPIGUI	() = default;


// Inherited via INativeGUI
	virtual bool				MainLoop		( bool blockingMode )							override;
	virtual bool				Init			( const NativeGUIInitData& initData )			override;
	virtual sw::input::IInput*	UseNativeInput	()												override;
	virtual INativeWindow*		CreateWindow	( NativeWindowDescriptor& descriptor )			override;

	static WinAPIGUI*			Create			();

private:
	void					RegisterWindowClass		();
	bool					MainLoopCore			( MSG* msg );

public:
	// Helpers
	static void				PrintLastError			();
	static const wchar_t*	GetWindowClassName		();

public:
	///@Internal
	///@{
	void                    HandleEvent 			( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	///@}
};

}	// GUI
}	// sw
