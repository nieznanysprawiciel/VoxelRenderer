#pragma once


#include "swGUI/Native/INativeGUI.h"


class WinApiInputProxy;

#include <Windows.h>
#undef CreateWindow

//struct tagMSG;
//typedef tagMSG MSG;
//
//struct HWND__;
//typedef HWND__* HWND;
//
//typedef UINT_PTR            WPARAM;
//typedef LONG_PTR            LPARAM;




namespace GUI
{


/**@brief */
class WinAPIGUI : public INativeGUI
{
private:
	WinApiInputProxy*		m_input;
	NativeGUIInitData		m_initData;

protected:
public:
	explicit			WinAPIGUI();
						~WinAPIGUI() = default;


	// Inherited via INativeGUI
	virtual bool				MainLoop		( bool blockingMode )							override;
	virtual bool				Init			( const NativeGUIInitData& initData )			override;
	virtual IInput*				UseNativeInput	()												override;
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
