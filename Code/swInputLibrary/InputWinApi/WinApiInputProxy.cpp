#include "WinApiInputProxy.h"
#include <Windowsx.h>


namespace sw {
namespace input
{



/**@brief */
WinApiInputProxy::WinApiInputProxy()
	:	m_lastX( 0 )
	,	m_lastY( 0 )
	,	m_eventNum( 0 )
{}

/**@brief */
WinApiInputProxy::~WinApiInputProxy()
{}

/**@brief */
bool WinApiInputProxy::Init( const InputInitInfo & initInfo )
{
	m_keyboards.push_back( KeyboardDeviceOPtr( new KeyboardDevice() ) );
	m_mouses.push_back( MouseDeviceOPtr( new MouseDevice() ) );
	m_joysticks.push_back( JoystickDeviceOPtr( new JoystickDevice() ) );

	m_keyboardsStates.push_back( &m_keyboards[ 0 ]->GetState() );
	m_mousesStates.push_back( &m_mouses[ 0 ]->GetState() );
	m_joysticksStates.push_back( &m_joysticks[ 0 ]->GetState() );

	return true;
}


/**@copydoc IInput::GetKeyboardStates*/
const std::vector< const KeyboardState* >&	WinApiInputProxy::GetKeyboardStates	() const
{
	return m_keyboardsStates;
}

/**@copydoc IInput::GetMouseStates*/
const std::vector< const MouseState* >&		WinApiInputProxy::GetMouseStates	() const
{
	return m_mousesStates;
}

/**@copydoc IInput::GetJoystickStates*/
const std::vector< const JoystickState* >&	WinApiInputProxy::GetJoystickStates	() const
{
	return m_joysticksStates;
}

/**@copydoc IInput::GetKeyboardDevice*/
std::vector< KeyboardDeviceOPtr >&		WinApiInputProxy::GetKeyboardDevice	()
{
	return m_keyboards;
}

/**@copydoc IInput::GetMouseDevice*/
std::vector< MouseDeviceOPtr >&			WinApiInputProxy::GetMouseDevice	()
{
	return m_mouses;
}

/**@copydoc IInput::GetJoystickDevice*/
std::vector< JoystickDeviceOPtr >&		WinApiInputProxy::GetJoystickDevice	()
{
	return m_joysticks;
}


/**@brief */
std::vector< const InputDeviceInfo* >	WinApiInputProxy::GetDevicesInfo	() const
{
	std::vector< const InputDeviceInfo* > infos;
	for( auto& device : m_keyboards )
		infos.push_back( &device->GetInfo() );
	for( auto& device : m_joysticks )
		infos.push_back( &device->GetInfo() );
	for( auto& device : m_mouses )
		infos.push_back( &device->GetInfo() );

	return infos;
}

/**@copydoc IInput::Update

Funkcja zapisuje pozycjê myszy z ostatniej klatki, ¿eby j¹ przerobiæ na informacjê o zmianach na osiach myszy.
WPF dostarcza tylko informacjê o po³o¿eniu myszy wzglêdem okna podgl¹du renderowania. Z tego przesuniêcie na osiach
trzeba sobie stworzyæ samemu.*/
void		WinApiInputProxy::Update( float timeInterval )
{
	m_eventNum = 0;

	for( auto& keyboard : m_keyboards )
		keyboard->RemoveEvents();

	for( auto& mouse : m_mouses )
		mouse->RemoveEvents();
}

/**@copydoc IInput::UpdateDevices

Urz¹dzenie jest zawsze aktualne.*/
bool		WinApiInputProxy::UpdateDevices()
{
	return true;
}

//====================================================================================//
//			WPF API	
//====================================================================================//

#define NUM_WINAPI_MOUSE_BUTTONS 7

/**@brief Tablica mapowania przycisków myszy WinApi na wartoœci silnikowe.*/
Mouse::PhysicalButtons MOUSE_BUTTONS_MAPPING[ NUM_WINAPI_MOUSE_BUTTONS ] =
{
	Mouse::PhysicalButtons::NONE,
	Mouse::PhysicalButtons::LEFT_BUTTON,
	Mouse::PhysicalButtons::RIGHT_BUTTON,
	Mouse::PhysicalButtons::NONE,
	Mouse::PhysicalButtons::MIDDLE_BUTTON,
	Mouse::PhysicalButtons::BUTTON3,
	Mouse::PhysicalButtons::BUTTON4
};

#define NUM_WINAPI_KEYBOARD_BUTTONS 173

/**@brief Tablica mapowania przycisków klawiatury WinApi na wartoœci silnikowe.*/
Keyboard::PhysicalKeys KEYBOARD_BUTTONS_MAPPING[ NUM_WINAPI_KEYBOARD_BUTTONS ] ={};	// Inicjujemy zerami, które przypadkowo s¹ równie¿ wartoœci¹ Keyboard::PhysicalKeys::KEY_NONE

// To jest odrobinê wygodniejsza metoda inicjalizowania, poniewa¿ nie trzeba wype³niaæ wszystkich miejsc od razu.
bool ArrayStaticInitializerFun()
{
	KEYBOARD_BUTTONS_MAPPING[ VK_BACK ]		= Keyboard::PhysicalKeys::KEY_BACKSPACE;
	KEYBOARD_BUTTONS_MAPPING[ VK_TAB ]		= Keyboard::PhysicalKeys::KEY_TAB;
	KEYBOARD_BUTTONS_MAPPING[ VK_RETURN ]	= Keyboard::PhysicalKeys::KEY_RETURN;
	KEYBOARD_BUTTONS_MAPPING[ VK_SHIFT ]	= Keyboard::PhysicalKeys::KEY_LSHIFT;		// ??
	KEYBOARD_BUTTONS_MAPPING[ VK_CONTROL ]	= Keyboard::PhysicalKeys::KEY_LCONTROL;	// ?? Left control and right control
	KEYBOARD_BUTTONS_MAPPING[ VK_MENU ]		= Keyboard::PhysicalKeys::KEY_LALT;		// ??
	KEYBOARD_BUTTONS_MAPPING[ VK_PAUSE ]	= Keyboard::PhysicalKeys::KEY_PAUSE;
	KEYBOARD_BUTTONS_MAPPING[ VK_CAPITAL ]	= Keyboard::PhysicalKeys::KEY_CAPSLOCK;
	KEYBOARD_BUTTONS_MAPPING[ VK_ESCAPE ]	= Keyboard::PhysicalKeys::KEY_ESCAPE;
	KEYBOARD_BUTTONS_MAPPING[ VK_SPACE ]	= Keyboard::PhysicalKeys::KEY_SPACE;
	KEYBOARD_BUTTONS_MAPPING[ VK_PRIOR ]	= Keyboard::PhysicalKeys::KEY_PAGEUP;
	KEYBOARD_BUTTONS_MAPPING[ VK_NEXT ]		= Keyboard::PhysicalKeys::KEY_PAGEDOWN;
	KEYBOARD_BUTTONS_MAPPING[ VK_END ]		= Keyboard::PhysicalKeys::KEY_END;
	KEYBOARD_BUTTONS_MAPPING[ VK_HOME ]		= Keyboard::PhysicalKeys::KEY_HOME;
	KEYBOARD_BUTTONS_MAPPING[ VK_LEFT ]		= Keyboard::PhysicalKeys::KEY_LEFTARROW;
	KEYBOARD_BUTTONS_MAPPING[ VK_UP ]		= Keyboard::PhysicalKeys::KEY_UP;
	KEYBOARD_BUTTONS_MAPPING[ VK_RIGHT ]	= Keyboard::PhysicalKeys::KEY_RIGHT;
	KEYBOARD_BUTTONS_MAPPING[ VK_DOWN ]		= Keyboard::PhysicalKeys::KEY_DOWN;
	KEYBOARD_BUTTONS_MAPPING[ VK_SNAPSHOT ]	= Keyboard::PhysicalKeys::KEY_PRINTSCREEN;
	KEYBOARD_BUTTONS_MAPPING[ VK_INSERT ]	= Keyboard::PhysicalKeys::KEY_INSERT;
	KEYBOARD_BUTTONS_MAPPING[ VK_DELETE ]	= Keyboard::PhysicalKeys::KEY_DELETE;

	KEYBOARD_BUTTONS_MAPPING[ '0' ]	= Keyboard::PhysicalKeys::KEY_0;
	KEYBOARD_BUTTONS_MAPPING[ '1' ]	= Keyboard::PhysicalKeys::KEY_1;
	KEYBOARD_BUTTONS_MAPPING[ '2' ]	= Keyboard::PhysicalKeys::KEY_2;
	KEYBOARD_BUTTONS_MAPPING[ '3' ]	= Keyboard::PhysicalKeys::KEY_3;
	KEYBOARD_BUTTONS_MAPPING[ '4' ]	= Keyboard::PhysicalKeys::KEY_4;
	KEYBOARD_BUTTONS_MAPPING[ '5' ]	= Keyboard::PhysicalKeys::KEY_5;
	KEYBOARD_BUTTONS_MAPPING[ '6' ]	= Keyboard::PhysicalKeys::KEY_6;
	KEYBOARD_BUTTONS_MAPPING[ '7' ]	= Keyboard::PhysicalKeys::KEY_7;
	KEYBOARD_BUTTONS_MAPPING[ '8' ]	= Keyboard::PhysicalKeys::KEY_8;
	KEYBOARD_BUTTONS_MAPPING[ '9' ]	= Keyboard::PhysicalKeys::KEY_9;

	KEYBOARD_BUTTONS_MAPPING[ 'A' ]	= Keyboard::PhysicalKeys::KEY_A;
	KEYBOARD_BUTTONS_MAPPING[ 'B' ]	= Keyboard::PhysicalKeys::KEY_B;
	KEYBOARD_BUTTONS_MAPPING[ 'C' ]	= Keyboard::PhysicalKeys::KEY_C;
	KEYBOARD_BUTTONS_MAPPING[ 'D' ]	= Keyboard::PhysicalKeys::KEY_D;
	KEYBOARD_BUTTONS_MAPPING[ 'E' ]	= Keyboard::PhysicalKeys::KEY_E;
	KEYBOARD_BUTTONS_MAPPING[ 'F' ]	= Keyboard::PhysicalKeys::KEY_F;
	KEYBOARD_BUTTONS_MAPPING[ 'G' ]	= Keyboard::PhysicalKeys::KEY_G;
	KEYBOARD_BUTTONS_MAPPING[ 'H' ]	= Keyboard::PhysicalKeys::KEY_H;
	KEYBOARD_BUTTONS_MAPPING[ 'I' ]	= Keyboard::PhysicalKeys::KEY_I;
	KEYBOARD_BUTTONS_MAPPING[ 'J' ]	= Keyboard::PhysicalKeys::KEY_J;
	KEYBOARD_BUTTONS_MAPPING[ 'K' ]	= Keyboard::PhysicalKeys::KEY_K;
	KEYBOARD_BUTTONS_MAPPING[ 'L' ]	= Keyboard::PhysicalKeys::KEY_L;
	KEYBOARD_BUTTONS_MAPPING[ 'M' ]	= Keyboard::PhysicalKeys::KEY_M;
	KEYBOARD_BUTTONS_MAPPING[ 'N' ]	= Keyboard::PhysicalKeys::KEY_N;
	KEYBOARD_BUTTONS_MAPPING[ 'O' ]	= Keyboard::PhysicalKeys::KEY_O;
	KEYBOARD_BUTTONS_MAPPING[ 'P' ]	= Keyboard::PhysicalKeys::KEY_P;
	KEYBOARD_BUTTONS_MAPPING[ 'Q' ]	= Keyboard::PhysicalKeys::KEY_Q;
	KEYBOARD_BUTTONS_MAPPING[ 'R' ]	= Keyboard::PhysicalKeys::KEY_R;
	KEYBOARD_BUTTONS_MAPPING[ 'S' ]	= Keyboard::PhysicalKeys::KEY_S;
	KEYBOARD_BUTTONS_MAPPING[ 'T' ]	= Keyboard::PhysicalKeys::KEY_T;
	KEYBOARD_BUTTONS_MAPPING[ 'U' ]	= Keyboard::PhysicalKeys::KEY_U;
	KEYBOARD_BUTTONS_MAPPING[ 'W' ]	= Keyboard::PhysicalKeys::KEY_W;
	KEYBOARD_BUTTONS_MAPPING[ 'X' ]	= Keyboard::PhysicalKeys::KEY_X;
	KEYBOARD_BUTTONS_MAPPING[ 'Y' ]	= Keyboard::PhysicalKeys::KEY_Y;
	KEYBOARD_BUTTONS_MAPPING[ 'Z' ]	= Keyboard::PhysicalKeys::KEY_Z;

	KEYBOARD_BUTTONS_MAPPING[ VK_LWIN ]		= Keyboard::PhysicalKeys::KEY_LWIN;
	KEYBOARD_BUTTONS_MAPPING[ VK_RWIN ]		= Keyboard::PhysicalKeys::KEY_RWIN;
	KEYBOARD_BUTTONS_MAPPING[ VK_APPS ]		= Keyboard::PhysicalKeys::KEY_APPS;
	KEYBOARD_BUTTONS_MAPPING[ VK_SLEEP ]	= Keyboard::PhysicalKeys::KEY_SLEEP;

	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD0 ]	= Keyboard::PhysicalKeys::KEY_NUMPAD0;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD1 ]	= Keyboard::PhysicalKeys::KEY_NUMPAD1;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD2 ]	= Keyboard::PhysicalKeys::KEY_NUMPAD2;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD3 ]	= Keyboard::PhysicalKeys::KEY_NUMPAD3;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD4 ]	= Keyboard::PhysicalKeys::KEY_NUMPAD4;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD5 ]	= Keyboard::PhysicalKeys::KEY_NUMPAD5;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD6 ]	= Keyboard::PhysicalKeys::KEY_NUMPAD6;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD7 ]	= Keyboard::PhysicalKeys::KEY_NUMPAD7;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD8 ]	= Keyboard::PhysicalKeys::KEY_NUMPAD8;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD9 ]	= Keyboard::PhysicalKeys::KEY_NUMPAD9;

	KEYBOARD_BUTTONS_MAPPING[ VK_MULTIPLY ]	= Keyboard::PhysicalKeys::KEY_MULTIPLY;
	KEYBOARD_BUTTONS_MAPPING[ VK_ADD ]		= Keyboard::PhysicalKeys::KEY_ADD;
	KEYBOARD_BUTTONS_MAPPING[ VK_SUBTRACT ]	= Keyboard::PhysicalKeys::KEY_SUBTRACT;
	KEYBOARD_BUTTONS_MAPPING[ VK_DECIMAL ]	= Keyboard::PhysicalKeys::KEY_DECIMAL;
	KEYBOARD_BUTTONS_MAPPING[ VK_DIVIDE ]	= Keyboard::PhysicalKeys::KEY_DIVIDE;

	KEYBOARD_BUTTONS_MAPPING[ VK_F1 ]		= Keyboard::PhysicalKeys::KEY_F1;
	KEYBOARD_BUTTONS_MAPPING[ VK_F2 ]		= Keyboard::PhysicalKeys::KEY_F2;
	KEYBOARD_BUTTONS_MAPPING[ VK_F3 ]		= Keyboard::PhysicalKeys::KEY_F3;
	KEYBOARD_BUTTONS_MAPPING[ VK_F4 ]		= Keyboard::PhysicalKeys::KEY_F4;
	KEYBOARD_BUTTONS_MAPPING[ VK_F5 ]		= Keyboard::PhysicalKeys::KEY_F5;
	KEYBOARD_BUTTONS_MAPPING[ VK_F6 ]		= Keyboard::PhysicalKeys::KEY_F6;
	KEYBOARD_BUTTONS_MAPPING[ VK_F7 ]		= Keyboard::PhysicalKeys::KEY_F7;
	KEYBOARD_BUTTONS_MAPPING[ VK_F8 ]		= Keyboard::PhysicalKeys::KEY_F8;
	KEYBOARD_BUTTONS_MAPPING[ VK_F9 ]		= Keyboard::PhysicalKeys::KEY_F9;
	KEYBOARD_BUTTONS_MAPPING[ VK_F10 ]		= Keyboard::PhysicalKeys::KEY_F10;
	KEYBOARD_BUTTONS_MAPPING[ VK_F11 ]		= Keyboard::PhysicalKeys::KEY_F11;
	KEYBOARD_BUTTONS_MAPPING[ VK_F12 ]		= Keyboard::PhysicalKeys::KEY_F12;
	KEYBOARD_BUTTONS_MAPPING[ VK_F13 ]		= Keyboard::PhysicalKeys::KEY_F13;
	KEYBOARD_BUTTONS_MAPPING[ VK_F14 ]		= Keyboard::PhysicalKeys::KEY_F14;
	KEYBOARD_BUTTONS_MAPPING[ VK_F15 ]		= Keyboard::PhysicalKeys::KEY_F15;

	KEYBOARD_BUTTONS_MAPPING[ VK_NUMLOCK ]	= Keyboard::PhysicalKeys::KEY_NUMLOCK;
	KEYBOARD_BUTTONS_MAPPING[ VK_SCROLL ]	= Keyboard::PhysicalKeys::KEY_SCROLL;
	KEYBOARD_BUTTONS_MAPPING[ VK_LCONTROL ]	= Keyboard::PhysicalKeys::KEY_LCONTROL;
	KEYBOARD_BUTTONS_MAPPING[ VK_RCONTROL ]	= Keyboard::PhysicalKeys::KEY_RCONTROL;

	KEYBOARD_BUTTONS_MAPPING[ VK_LMENU ]	= Keyboard::PhysicalKeys::KEY_LMENU;
	KEYBOARD_BUTTONS_MAPPING[ VK_RMENU ]	= Keyboard::PhysicalKeys::KEY_RMENU;
	KEYBOARD_BUTTONS_MAPPING[ VK_RSHIFT ]	= Keyboard::PhysicalKeys::KEY_RSHIFT;
	KEYBOARD_BUTTONS_MAPPING[ VK_LSHIFT ]	= Keyboard::PhysicalKeys::KEY_LSHIFT;

	KEYBOARD_BUTTONS_MAPPING[ VK_RSHIFT ]	= Keyboard::PhysicalKeys::KEY_RSHIFT;

	return false;
}
static bool arrayStaticInitializer = ArrayStaticInitializerFun();



/**@brief Ustawia nowy stan przycisku na klawiaturze.

@todo W przysz³oœci mo¿e trzeba bêdzie dodaæ informacjê o zmienie stanu.
Móg³by to byæ np któryæ bit ustawiony na 1 czy coœ.*/
void			WinApiInputProxy::KeyboardChange		( int keyId, bool pressed )
{
	KeyEvent keyEvent;
	keyEvent.Key = KEYBOARD_BUTTONS_MAPPING[ keyId ];
	keyEvent.State = pressed;

	m_keyboards[ 0 ]->AddEvent( DeviceEvent( keyEvent, m_eventNum++ ) );
}

// ================================ //
//
void			WinApiInputProxy::KeyboardCharacter		( wchar_t character )
{
	CharacterEvent charEvent;
	charEvent.Character = character;

	m_keyboards[ 0 ]->AddEvent( DeviceEvent( charEvent, m_eventNum++ ) );
}


/**@brief Ustawia nowy stan przycisku myszy.

@todo W przysz³oœci mo¿e trzeba bêdzie dodaæ informacjê o zmienie stanu.
Móg³by to byæ np któryæ bit ustawiony na 1 czy coœ.*/
void			WinApiInputProxy::MouseButtonChange		( int button, bool pressed )
{
	ButtonEvent mouseButtonEvt;
	mouseButtonEvt.Button = MOUSE_BUTTONS_MAPPING[ button ];
	mouseButtonEvt.State = pressed;

	m_mouses[ 0 ]->AddEvent( DeviceEvent( mouseButtonEvt, m_eventNum++ ) );
}

/**@brief Ustawia now¹ pozycjê myszy.*/
void			WinApiInputProxy::MousePositionChange	( short X, short Y )
{
	auto& mouse = m_mousesStates[ 0 ];
	
	CursorEvent cursorEvent;
	cursorEvent.OffsetX = X - m_lastX;
	cursorEvent.OffsetY = Y - m_lastY;

	m_mouses[ 0 ]->AddEvent( DeviceEvent( cursorEvent, m_eventNum++ ) );

	m_lastX = X;
	m_lastY = Y;

	// Change in mouse position is translated into axis change.
	// Theoretically it's the same event but better give it a new number.
	AxisEvent cursorAxisX;
	cursorAxisX.Delta = cursorEvent.OffsetX;
	cursorAxisX.Axis = Mouse::PhysicalAxes::X_AXIS;

	m_mouses[ 0 ]->AddEvent( DeviceEvent( cursorAxisX, m_eventNum++ ) );

	AxisEvent cursorAxisY;
	cursorAxisY.Delta = cursorEvent.OffsetY;
	cursorAxisY.Axis = Mouse::PhysicalAxes::Y_AXIS;

	m_mouses[ 0 ]->AddEvent( DeviceEvent( cursorAxisY, m_eventNum++ ) );
}

/**@brief Ustawia przesuniêcie kó³ka myszy.*/
void			WinApiInputProxy::MouseWheelChange		( double delta )
{
	AxisEvent wheel;
	wheel.Delta = (float)delta;
	wheel.Axis = Mouse::PhysicalAxes::WHEEL;

	m_mouses[ 0 ]->AddEvent( DeviceEvent( wheel, m_eventNum++ ) );
}

/**@brief Poniewa¿ okno straci³o focus to czyœcimy stan przycisków i myszy.

@todo Dokoñczyæ*/
void			WinApiInputProxy::LostFocus()
{

}

/**@brief Funkcja uzupe³nia stan klasy bezpoœrednio na podstawie komunikatów z WinAPI.

Je¿eli siê u¿ywa tej funkcji nie nale¿y ju¿ wywo³ywaæ pozosta³ych funkcji.*/
void			WinApiInputProxy::HandleEvent			( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_MOUSEMOVE:
		{
			int xPos = GET_X_LPARAM( lParam );
			int yPos = GET_Y_LPARAM( lParam );
			MousePositionChange( xPos, yPos );

			break;
		}
		case WM_KEYDOWN:
			KeyboardChange( (int)wParam, true );
			break;
		case WM_KEYUP:
			KeyboardChange( (int)wParam, false );
			break;
		case WM_RBUTTONDOWN:
			MouseButtonChange( (int)VK_RBUTTON, true );
			break;
		case WM_LBUTTONDOWN:
			MouseButtonChange( (int)VK_LBUTTON, true );
			break;
		case WM_MBUTTONDOWN:
			MouseButtonChange( (int)VK_MBUTTON, true );
			break;
		case WM_XBUTTONDOWN:
		{
			auto button = GET_XBUTTON_WPARAM( wParam );
			MouseButtonChange( (int)( VK_MBUTTON + button ), true );	/// GET_XBUTTON_WPARAM returns 1 on XButton1 and 2 on XButton2.
			break;
		}
		case WM_RBUTTONUP:
			MouseButtonChange( (int)VK_RBUTTON, false );
			break;
		case WM_LBUTTONUP:
			MouseButtonChange( (int)VK_LBUTTON, false );
			break;
		case WM_MBUTTONUP:
			MouseButtonChange( (int)VK_MBUTTON, false );
			break;
		case WM_XBUTTONUP:
		{
			auto button = GET_XBUTTON_WPARAM( wParam );
			MouseButtonChange( (int)( VK_MBUTTON + button ), false );	/// GET_XBUTTON_WPARAM returns 1 on XButton1 and 2 on XButton2.
			break;
		}
		case WM_MOUSEWHEEL:
		{
			MouseWheelChange( GET_WHEEL_DELTA_WPARAM( wParam ) );
			break;
		}
		case WM_CHAR:
			KeyboardCharacter( (wchar_t)wParam );
			break;
		default:
			break;
	}

}



}	// input
}	// sw

