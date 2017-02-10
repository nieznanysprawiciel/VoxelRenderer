#include "WinApiInputProxy.h"
#include <Windowsx.h>

/**@brief */
WinApiInputProxy::WinApiInputProxy()
	:	m_lastX( 0.0 )
	,	m_lastY( 0.0 )
{ }

/**@brief */
WinApiInputProxy::~WinApiInputProxy()
{ }

/**@brief */
bool WinApiInputProxy::Init( const InputInitInfo & initInfo )
{
	m_keyboards.push_back( new KeyboardState() );
	m_mouses.push_back( new MouseState() );
	m_joysticks.push_back( new JoystickState() );

	return true;
}


/**@copydoc IInput::GetKeyboardStates*/
const std::vector< KeyboardState* >&	WinApiInputProxy::GetKeyboardStates	()
{
	return m_keyboards;
}

/**@copydoc IInput::GetMouseStates*/
const std::vector< MouseState* >&		WinApiInputProxy::GetMouseStates		()
{
	return m_mouses;
}

/**@copydoc IInput::GetJoystickStates*/
const std::vector< JoystickState* >&	WinApiInputProxy::GetJoystickStates	()
{
	return m_joysticks;
}

/**@brief */
std::vector< const InputDeviceInfo* >	WinApiInputProxy::GetDevicesInfo		()
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
void WinApiInputProxy::Update( float timeInterval )
{
	auto& mouse = m_mouses[ 0 ];
	auto axes = mouse->GetAxesState();

	axes[ MouseState::PHYSICAL_AXES::X_AXIS ] = static_cast< float >( mouse->GetPositionX() - m_lastX );
	axes[ MouseState::PHYSICAL_AXES::Y_AXIS ] = static_cast< float >( mouse->GetPositionY() - m_lastY );

	m_lastX = mouse->GetPositionX();
	m_lastY = mouse->GetPositionY();

	for( auto& keyboard : m_keyboards )
		keyboard->RemoveEvents();

	for( auto& mouse : m_mouses )
		mouse->RemoveEvents();
}

/**@copydoc IInput::UpdateDevices

Urz¹dzenie jest zawsze aktualne.*/
bool WinApiInputProxy::UpdateDevices()
{	return true;	}

//====================================================================================//
//			WPF API	
//====================================================================================//

#define NUM_WINAPI_MOUSE_BUTTONS 7

/**@brief Tablica mapowania przycisków myszy WinApi na wartoœci silnikowe.*/
MouseState::PHYSICAL_BUTTONS MOUSE_BUTTONS_MAPPING[ NUM_WINAPI_MOUSE_BUTTONS ] =
{
	MouseState::PHYSICAL_BUTTONS::NONE,
	MouseState::PHYSICAL_BUTTONS::LEFT_BUTTON,
	MouseState::PHYSICAL_BUTTONS::RIGHT_BUTTON,
	MouseState::PHYSICAL_BUTTONS::NONE,
	MouseState::PHYSICAL_BUTTONS::MIDDLE_BUTTON,
	MouseState::PHYSICAL_BUTTONS::BUTTON3,
	MouseState::PHYSICAL_BUTTONS::BUTTON4
};

#define NUM_WINAPI_KEYBOARD_BUTTONS 173

/**@brief Tablica mapowania przycisków klawiatury WinApi na wartoœci silnikowe.*/
KeyboardState::PHYSICAL_KEYS KEYBOARD_BUTTONS_MAPPING[ NUM_WINAPI_KEYBOARD_BUTTONS ] =	{};	// Inicjujemy zerami, które przypadkowo s¹ równie¿ wartoœci¹ KeyboardState::PHYSICAL_KEYS::KEY_NONE

// To jest odrobinê wygodniejsza metoda inicjalizowania, poniewa¿ nie trzeba wype³niaæ wszystkich miejsc od razu.
bool ArrayStaticInitializerFun()
{
	KEYBOARD_BUTTONS_MAPPING[ VK_BACK ]		= KeyboardState::PHYSICAL_KEYS::KEY_BACKSPACE;
	KEYBOARD_BUTTONS_MAPPING[ VK_TAB ]		= KeyboardState::PHYSICAL_KEYS::KEY_TAB;
	KEYBOARD_BUTTONS_MAPPING[ VK_RETURN ]	= KeyboardState::PHYSICAL_KEYS::KEY_RETURN;
	KEYBOARD_BUTTONS_MAPPING[ VK_SHIFT ]	= KeyboardState::PHYSICAL_KEYS::KEY_LSHIFT;		// ??
	KEYBOARD_BUTTONS_MAPPING[ VK_CONTROL ]	= KeyboardState::PHYSICAL_KEYS::KEY_LCONTROL;	// ?? Left control and right control
	KEYBOARD_BUTTONS_MAPPING[ VK_MENU ]		= KeyboardState::PHYSICAL_KEYS::KEY_LALT;		// ??
	KEYBOARD_BUTTONS_MAPPING[ VK_PAUSE ]	= KeyboardState::PHYSICAL_KEYS::KEY_PAUSE;
	KEYBOARD_BUTTONS_MAPPING[ VK_CAPITAL ]	= KeyboardState::PHYSICAL_KEYS::KEY_CAPSLOCK;
	KEYBOARD_BUTTONS_MAPPING[ VK_ESCAPE ]	= KeyboardState::PHYSICAL_KEYS::KEY_ESCAPE;
	KEYBOARD_BUTTONS_MAPPING[ VK_SPACE ]	= KeyboardState::PHYSICAL_KEYS::KEY_SPACE;
	KEYBOARD_BUTTONS_MAPPING[ VK_PRIOR ]	= KeyboardState::PHYSICAL_KEYS::KEY_PAGEUP;
	KEYBOARD_BUTTONS_MAPPING[ VK_NEXT ]		= KeyboardState::PHYSICAL_KEYS::KEY_PAGEDOWN;
	KEYBOARD_BUTTONS_MAPPING[ VK_END ]		= KeyboardState::PHYSICAL_KEYS::KEY_END;
	KEYBOARD_BUTTONS_MAPPING[ VK_HOME ]		= KeyboardState::PHYSICAL_KEYS::KEY_HOME;
	KEYBOARD_BUTTONS_MAPPING[ VK_LEFT ]		= KeyboardState::PHYSICAL_KEYS::KEY_LEFTARROW;
	KEYBOARD_BUTTONS_MAPPING[ VK_UP ]		= KeyboardState::PHYSICAL_KEYS::KEY_UP;
	KEYBOARD_BUTTONS_MAPPING[ VK_RIGHT ]	= KeyboardState::PHYSICAL_KEYS::KEY_RIGHT;
	KEYBOARD_BUTTONS_MAPPING[ VK_DOWN ]		= KeyboardState::PHYSICAL_KEYS::KEY_DOWN;
	KEYBOARD_BUTTONS_MAPPING[ VK_SNAPSHOT ]	= KeyboardState::PHYSICAL_KEYS::KEY_PRINTSCREEN;
	KEYBOARD_BUTTONS_MAPPING[ VK_INSERT ]	= KeyboardState::PHYSICAL_KEYS::KEY_INSERT;
	KEYBOARD_BUTTONS_MAPPING[ VK_DELETE ]	= KeyboardState::PHYSICAL_KEYS::KEY_DELETE;

	KEYBOARD_BUTTONS_MAPPING[ '0' ]	= KeyboardState::PHYSICAL_KEYS::KEY_0;
	KEYBOARD_BUTTONS_MAPPING[ '1' ]	= KeyboardState::PHYSICAL_KEYS::KEY_1;
	KEYBOARD_BUTTONS_MAPPING[ '2' ]	= KeyboardState::PHYSICAL_KEYS::KEY_2;
	KEYBOARD_BUTTONS_MAPPING[ '3' ]	= KeyboardState::PHYSICAL_KEYS::KEY_3;
	KEYBOARD_BUTTONS_MAPPING[ '4' ]	= KeyboardState::PHYSICAL_KEYS::KEY_4;
	KEYBOARD_BUTTONS_MAPPING[ '5' ]	= KeyboardState::PHYSICAL_KEYS::KEY_5;
	KEYBOARD_BUTTONS_MAPPING[ '6' ]	= KeyboardState::PHYSICAL_KEYS::KEY_6;
	KEYBOARD_BUTTONS_MAPPING[ '7' ]	= KeyboardState::PHYSICAL_KEYS::KEY_7;
	KEYBOARD_BUTTONS_MAPPING[ '8' ]	= KeyboardState::PHYSICAL_KEYS::KEY_8;
	KEYBOARD_BUTTONS_MAPPING[ '9' ]	= KeyboardState::PHYSICAL_KEYS::KEY_9;

	KEYBOARD_BUTTONS_MAPPING[ 'A' ]	= KeyboardState::PHYSICAL_KEYS::KEY_A;
	KEYBOARD_BUTTONS_MAPPING[ 'B' ]	= KeyboardState::PHYSICAL_KEYS::KEY_B;
	KEYBOARD_BUTTONS_MAPPING[ 'C' ]	= KeyboardState::PHYSICAL_KEYS::KEY_C;
	KEYBOARD_BUTTONS_MAPPING[ 'D' ]	= KeyboardState::PHYSICAL_KEYS::KEY_D;
	KEYBOARD_BUTTONS_MAPPING[ 'E' ]	= KeyboardState::PHYSICAL_KEYS::KEY_E;
	KEYBOARD_BUTTONS_MAPPING[ 'F' ]	= KeyboardState::PHYSICAL_KEYS::KEY_F;
	KEYBOARD_BUTTONS_MAPPING[ 'G' ]	= KeyboardState::PHYSICAL_KEYS::KEY_G;
	KEYBOARD_BUTTONS_MAPPING[ 'H' ]	= KeyboardState::PHYSICAL_KEYS::KEY_H;
	KEYBOARD_BUTTONS_MAPPING[ 'I' ]	= KeyboardState::PHYSICAL_KEYS::KEY_I;
	KEYBOARD_BUTTONS_MAPPING[ 'J' ]	= KeyboardState::PHYSICAL_KEYS::KEY_J;
	KEYBOARD_BUTTONS_MAPPING[ 'K' ]	= KeyboardState::PHYSICAL_KEYS::KEY_K;
	KEYBOARD_BUTTONS_MAPPING[ 'L' ]	= KeyboardState::PHYSICAL_KEYS::KEY_L;
	KEYBOARD_BUTTONS_MAPPING[ 'M' ]	= KeyboardState::PHYSICAL_KEYS::KEY_M;
	KEYBOARD_BUTTONS_MAPPING[ 'N' ]	= KeyboardState::PHYSICAL_KEYS::KEY_N;
	KEYBOARD_BUTTONS_MAPPING[ 'O' ]	= KeyboardState::PHYSICAL_KEYS::KEY_O;
	KEYBOARD_BUTTONS_MAPPING[ 'P' ]	= KeyboardState::PHYSICAL_KEYS::KEY_P;
	KEYBOARD_BUTTONS_MAPPING[ 'Q' ]	= KeyboardState::PHYSICAL_KEYS::KEY_Q;
	KEYBOARD_BUTTONS_MAPPING[ 'R' ]	= KeyboardState::PHYSICAL_KEYS::KEY_R;
	KEYBOARD_BUTTONS_MAPPING[ 'S' ]	= KeyboardState::PHYSICAL_KEYS::KEY_S;
	KEYBOARD_BUTTONS_MAPPING[ 'T' ]	= KeyboardState::PHYSICAL_KEYS::KEY_T;
	KEYBOARD_BUTTONS_MAPPING[ 'U' ]	= KeyboardState::PHYSICAL_KEYS::KEY_U;
	KEYBOARD_BUTTONS_MAPPING[ 'W' ]	= KeyboardState::PHYSICAL_KEYS::KEY_W;
	KEYBOARD_BUTTONS_MAPPING[ 'X' ]	= KeyboardState::PHYSICAL_KEYS::KEY_X;
	KEYBOARD_BUTTONS_MAPPING[ 'Y' ]	= KeyboardState::PHYSICAL_KEYS::KEY_Y;
	KEYBOARD_BUTTONS_MAPPING[ 'Z' ]	= KeyboardState::PHYSICAL_KEYS::KEY_Z;
	
	KEYBOARD_BUTTONS_MAPPING[ VK_LWIN ]		= KeyboardState::PHYSICAL_KEYS::KEY_LWIN;
	KEYBOARD_BUTTONS_MAPPING[ VK_RWIN ]		= KeyboardState::PHYSICAL_KEYS::KEY_RWIN;
	KEYBOARD_BUTTONS_MAPPING[ VK_APPS ]		= KeyboardState::PHYSICAL_KEYS::KEY_APPS;
	KEYBOARD_BUTTONS_MAPPING[ VK_SLEEP ]	= KeyboardState::PHYSICAL_KEYS::KEY_SLEEP;

	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD0 ]	= KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD0;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD1 ]	= KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD1;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD2 ]	= KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD2;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD3 ]	= KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD3;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD4 ]	= KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD4;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD5 ]	= KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD5;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD6 ]	= KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD6;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD7 ]	= KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD7;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD8 ]	= KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD8;
	KEYBOARD_BUTTONS_MAPPING[ VK_NUMPAD9 ]	= KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD9;

	KEYBOARD_BUTTONS_MAPPING[ VK_MULTIPLY ]	= KeyboardState::PHYSICAL_KEYS::KEY_MULTIPLY;
	KEYBOARD_BUTTONS_MAPPING[ VK_ADD ]		= KeyboardState::PHYSICAL_KEYS::KEY_ADD;
	KEYBOARD_BUTTONS_MAPPING[ VK_SUBTRACT ]	= KeyboardState::PHYSICAL_KEYS::KEY_SUBTRACT;
	KEYBOARD_BUTTONS_MAPPING[ VK_DECIMAL ]	= KeyboardState::PHYSICAL_KEYS::KEY_DECIMAL;
	KEYBOARD_BUTTONS_MAPPING[ VK_DIVIDE ]	= KeyboardState::PHYSICAL_KEYS::KEY_DIVIDE;

	KEYBOARD_BUTTONS_MAPPING[ VK_F1 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F1;
	KEYBOARD_BUTTONS_MAPPING[ VK_F2 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F2;
	KEYBOARD_BUTTONS_MAPPING[ VK_F3 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F3;
	KEYBOARD_BUTTONS_MAPPING[ VK_F4 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F4;
	KEYBOARD_BUTTONS_MAPPING[ VK_F5 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F5;
	KEYBOARD_BUTTONS_MAPPING[ VK_F6 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F6;
	KEYBOARD_BUTTONS_MAPPING[ VK_F7 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F7;
	KEYBOARD_BUTTONS_MAPPING[ VK_F8 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F8;
	KEYBOARD_BUTTONS_MAPPING[ VK_F9 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F9;
	KEYBOARD_BUTTONS_MAPPING[ VK_F10 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F10;
	KEYBOARD_BUTTONS_MAPPING[ VK_F11 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F11;
	KEYBOARD_BUTTONS_MAPPING[ VK_F12 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F12;
	KEYBOARD_BUTTONS_MAPPING[ VK_F13 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F13;
	KEYBOARD_BUTTONS_MAPPING[ VK_F14 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F14;
	KEYBOARD_BUTTONS_MAPPING[ VK_F15 ]		= KeyboardState::PHYSICAL_KEYS::KEY_F15;

	KEYBOARD_BUTTONS_MAPPING[ VK_NUMLOCK ]	= KeyboardState::PHYSICAL_KEYS::KEY_NUMLOCK;
	KEYBOARD_BUTTONS_MAPPING[ VK_SCROLL ]	= KeyboardState::PHYSICAL_KEYS::KEY_SCROLL;
	KEYBOARD_BUTTONS_MAPPING[ VK_LCONTROL ]	= KeyboardState::PHYSICAL_KEYS::KEY_LCONTROL;
	KEYBOARD_BUTTONS_MAPPING[ VK_RCONTROL ]	= KeyboardState::PHYSICAL_KEYS::KEY_RCONTROL;

	KEYBOARD_BUTTONS_MAPPING[ VK_LMENU ]	= KeyboardState::PHYSICAL_KEYS::KEY_LMENU;
	KEYBOARD_BUTTONS_MAPPING[ VK_RMENU ]	= KeyboardState::PHYSICAL_KEYS::KEY_RMENU;
	KEYBOARD_BUTTONS_MAPPING[ VK_RSHIFT ]	= KeyboardState::PHYSICAL_KEYS::KEY_RSHIFT;
	KEYBOARD_BUTTONS_MAPPING[ VK_LSHIFT ]	= KeyboardState::PHYSICAL_KEYS::KEY_LSHIFT;

	KEYBOARD_BUTTONS_MAPPING[ VK_RSHIFT ]	= KeyboardState::PHYSICAL_KEYS::KEY_RSHIFT;

	return false;
}
static bool arrayStaticInitializer = ArrayStaticInitializerFun();

/**@brief Ustawia nowy stan przycisku na klawiaturze.

@todo W przysz³oœci mo¿e trzeba bêdzie dodaæ informacjê o zmienie stanu.
Móg³by to byæ np któryæ bit ustawiony na 1 czy coœ.*/
void WinApiInputProxy::KeyboardChange( int keyId, bool pressed )
{
	auto& keyboard = m_keyboards[ 0 ];
	auto state = keyboard->KeysState();
	state[ KEYBOARD_BUTTONS_MAPPING[ keyId ] ] = pressed;
}

/**@brief Ustawia nowy stan przycisku myszy.

@todo W przysz³oœci mo¿e trzeba bêdzie dodaæ informacjê o zmienie stanu.
Móg³by to byæ np któryæ bit ustawiony na 1 czy coœ.*/
void WinApiInputProxy::MouseButtonChange( int button, bool pressed )
{
	auto& mouse = m_mouses[ 0 ];
	auto buttonsState = mouse->GetButtonsState();
	buttonsState[ MOUSE_BUTTONS_MAPPING[ button ] ] = (char)pressed;
}

/**@brief Ustawia now¹ pozycjê myszy.*/
void WinApiInputProxy::MousePositionChange( short X, short Y )
{
	auto& mouse = m_mouses[ 0 ];
	mouse->SetPosition( (short)X, (short)Y );
}

/**@brief Ustawia przesuniêcie kó³ka myszy.*/
void WinApiInputProxy::MouseWheelChange( double delta )
{
	auto& mouse = m_mouses[ 0 ];
	mouse->GetAxesState()[ MouseState::PHYSICAL_AXES::WHEEL ] = (float)delta;
}

/**@brief Poniewa¿ okno straci³o focus to czyœcimy stan przycisków i myszy.

@todo Dokoñczyæ*/
void WinApiInputProxy::LostFocus()
{

}

/**@brief Funkcja uzupe³nia stan klasy bezpoœrednio na podstawie komunikatów z WinAPI.

Je¿eli siê u¿ywa tej funkcji nie nale¿y ju¿ wywo³ywaæ pozosta³ych funkcji.*/
void WinApiInputProxy::HandleEvent( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if( message == WM_MOUSEMOVE )
	{
		int xPos = GET_X_LPARAM( lParam );
		int yPos = GET_Y_LPARAM( lParam );
		MousePositionChange( xPos, yPos );
	}
	else if( message == WM_KEYDOWN )
	{
		if( wParam <= VK_XBUTTON2 )
			MouseButtonChange( (int)wParam, true );
		KeyboardChange( (int)wParam, true );
	}
	else if( message == WM_KEYUP )
	{
		if( wParam <= VK_XBUTTON2 )
			MouseButtonChange( (int)wParam, false );
		KeyboardChange( (int)wParam, false );
	}
	else if( message == WM_MOUSEWHEEL )
	{
		MouseWheelChange( GET_WHEEL_DELTA_WPARAM( wParam ) );
	}
}


