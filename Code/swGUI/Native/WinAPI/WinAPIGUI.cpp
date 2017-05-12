#include "WinAPIGUI.h"
#include "Win32ApiWindow.h"

#include "swInputLibrary/InputWinApi/WinApiInputProxy.h"
#include <Windows.h>



namespace sw {
namespace gui
{


LRESULT CALLBACK		WindowProcedure		( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INativeWindow*			GetNativePointer	( HWND window );
WinAPIGUI*				GetNativeAPIPointer	( HWND window );



// ================================ //
//
WinAPIGUI::WinAPIGUI()
	: m_input( nullptr )
{ }


/**@brief Creates WinAPIGUI object.*/
WinAPIGUI*		WinAPIGUI::Create()
{
	return new WinAPIGUI();
}



//====================================================================================//
//			Window creation	
//====================================================================================//


const wchar_t WINDOW_CLASS_NAME[] = L"gui Window";

/**@brief Window class registered in WinAPI.*/
const wchar_t*	WinAPIGUI::GetWindowClassName()
{
	return WINDOW_CLASS_NAME;
}

// ================================ //
//
sw::input::IInput*		WinAPIGUI::UseNativeInput()
{
	sw::input::InputInitInfo init;
	init.AppInstance = GetModuleHandle( nullptr );
	init.WndHandle = nullptr;

	m_input = new sw::input::WinApiInputProxy();
	if( !m_input->Init( init ) )
	{
		assert( !"Initialziation failed" );
		return nullptr;
	}

	return m_input;
}

// WinAPI macros :(
#undef CreateWindow

/**@copydoc INativeGUI::CreateWindow*/
INativeWindow*	WinAPIGUI::CreateWindow		( NativeWindowDescriptor& descriptor )
{
	Win32ApiWindow* newWindow = Win32ApiWindow::CreateWindowInstance( descriptor );
	HWND newWindowHandle = (HWND)newWindow->GetHandle();

	if( GetNativeAPIPointer( newWindowHandle ) == nullptr )
	{
		SetClassLongPtr( newWindowHandle, 0, ( LONG_PTR )this );
	}

	return newWindow;
}


//====================================================================================//
//			Initialization	
//====================================================================================//


/**@brief Registers window class.*/
void		WinAPIGUI::RegisterWindowClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProcedure;
	wcex.cbClsExtra = sizeof( WinAPIGUI* );
	wcex.cbWndExtra = sizeof( Win32ApiWindow* );
	wcex.hInstance = GetModuleHandle( nullptr );
	wcex.hIcon = LoadIcon( wcex.hInstance, IDI_APPLICATION );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = WINDOW_CLASS_NAME;
	wcex.hIconSm = LoadIcon( wcex.hInstance, IDI_APPLICATION );

	auto result = RegisterClassEx( &wcex );
	if( result == 0 )
		PrintLastError();
}


/**@brief Gets last win api error and prints to debug window.*/
void		WinAPIGUI::PrintLastError()
{
	LPVOID messageBuffer;
	DWORD error = GetLastError();
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				   NULL, error, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
				   (LPTSTR)&messageBuffer, 0, NULL );
	OutputDebugString( (LPCTSTR)messageBuffer );
	LocalFree( messageBuffer );
}

/**@brief Retrives INativeWindow pointer hidden in native window extra bytes.*/
INativeWindow*		GetNativePointer	( HWND window )
{
	return (INativeWindow*)GetWindowLongPtr( window, 0 );
}

/**@brief Retrives WinAPIGUI pointer hidden in native window extra bytes.*/
WinAPIGUI*			GetNativeAPIPointer	( HWND window )
{
	return (WinAPIGUI*)GetClassLongPtr( window, 0 );
}


/**@copydoc INativeGUI::Init*/
bool		WinAPIGUI::Init				( const NativeGUIInitData& initData )
{
	assert( !initData.FocusChanged.empty() );

	m_initData = initData;
	RegisterWindowClass();

	return true;
}



//====================================================================================//
//			Window Message handler
//====================================================================================//

/**@brief Core functionality of main loop function.*/
bool		WinAPIGUI::MainLoopCore             ( MSG* msg )
{
	TranslateMessage( msg );

	if( m_input )
		m_input->HandleEvent( msg->hwnd, msg->message, msg->wParam, msg->lParam );

	// Don't process same message two times.
	//HandleEvent( msg->hwnd, msg->message, msg->wParam, msg->lParam );

	DispatchMessage( msg );

	if( msg->message == WM_QUIT )
	{
		return true;
	}
	return false;
}

/**@brief Captures important events like changing focus.*/
void        WinAPIGUI::HandleEvent              ( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_SETFOCUS:
			m_initData.FocusChanged( GetNativePointer( hWnd ), true );
			break;
		case WM_KILLFOCUS:
			m_initData.FocusChanged( GetNativePointer( hWnd ), false );
			break;
		case WM_ACTIVATE:
			m_initData.FocusChanged( GetNativePointer( hWnd ), wParam != 0 );
			break;
		//case WM_MOUSEACTIVATE:
		//	m_initData.FocusChanged( GetNativePointer( hWnd ), true );
  //          break;
	}
}


/**@copydoc INativeGUI::MainLoop*/
bool		WinAPIGUI::MainLoop                 ( bool blockingMode )
{
	MSG msg;

	if( blockingMode )
	{
		GetMessage( &msg, NULL, 0, 0 );
		do
		{
			bool end = MainLoopCore( &msg );
			if( end ) return true;

		} while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) );
	}
	else
	{
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			bool end = MainLoopCore( &msg );
			if( end ) return true;
		}
	}

	return false;
}


/**@brief Window procedure.*/
LRESULT CALLBACK		WindowProcedure		( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch( message )
	{
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			// TODO: Add any drawing code here...
			EndPaint( hWnd, &ps );
			break;
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		case WM_ACTIVATE:
		{
			auto nativeGUI = GetNativeAPIPointer( hWnd );
			if( nativeGUI )
				nativeGUI->HandleEvent( hWnd, message, wParam, lParam );
			break;
		}
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}


}	// gui
}	// sw
