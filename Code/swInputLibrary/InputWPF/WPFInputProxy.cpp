/**
@file WPFInputProxy.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "WPFInputProxy.h"


/**@brief */
WPFInputProxy::WPFInputProxy()
	:	m_lastX( 0.0 )
	,	m_lastY( 0.0 )
{ }

/**@brief */
WPFInputProxy::~WPFInputProxy()
{ }

/**@brief */
bool WPFInputProxy::Init( const InputInitInfo & initInfo )
{
	m_keyboards.push_back( new KeyboardState() );
	m_mouses.push_back( new MouseState() );
	m_joysticks.push_back( new JoystickState() );

	return true;
}


/**@copydoc IInput::GetKeyboardStates*/
const std::vector< KeyboardState* >&	WPFInputProxy::GetKeyboardStates	()
{
	return m_keyboards;
}

/**@copydoc IInput::GetMouseStates*/
const std::vector< MouseState* >&		WPFInputProxy::GetMouseStates		()
{
	return m_mouses;
}

/**@copydoc IInput::GetJoystickStates*/
const std::vector< JoystickState* >&	WPFInputProxy::GetJoystickStates	()
{
	return m_joysticks;
}

/**@brief */
std::vector< const InputDeviceInfo* >	WPFInputProxy::GetDevicesInfo		()
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
void WPFInputProxy::Update( float timeInterval )
{
	auto& mouse = m_mouses[ 0 ];
	auto axes = mouse->GetAxesState();

	axes[ MouseState::PHYSICAL_AXES::X_AXIS ] = static_cast< float >( mouse->GetPositionX() - m_lastX );
	axes[ MouseState::PHYSICAL_AXES::Y_AXIS ] = static_cast< float >( mouse->GetPositionY() - m_lastY );

	m_lastX = mouse->GetPositionX();
	m_lastY = mouse->GetPositionY();
}

/**@copydoc IInput::UpdateDevices

Urz¹dzenie jest zawsze aktualne.*/
bool WPFInputProxy::UpdateDevices()
{	return true;	}

//====================================================================================//
//			WPF API	
//====================================================================================//

#define NUM_WPF_MOUSE_BUTTONS 5

/**@brief Tablica mapowania przycisków myszy WPFa na wartoœci silnikowe.

namespace System.Windows.Input
{
	public enum MouseButton
	{
		Left = 0,
		Middle = 1,
		Right = 2,
		XButton1 = 3,
		XButton2 = 4
	}
}*/
MouseState::PHYSICAL_BUTTONS MOUSE_BUTTONS_MAPPING[ NUM_WPF_MOUSE_BUTTONS ] =
{
	MouseState::PHYSICAL_BUTTONS::LEFT_BUTTON,
	MouseState::PHYSICAL_BUTTONS::MIDDLE_BUTTON,
	MouseState::PHYSICAL_BUTTONS::RIGHT_BUTTON,
	MouseState::PHYSICAL_BUTTONS::BUTTON3,
	MouseState::PHYSICAL_BUTTONS::BUTTON4
};

#define NUM_WPF_KEYBOARD_BUTTONS 173

/**@brief Tablica mapowania przycisków klawiatury WPFa na wartoœci silnikowe.*/
KeyboardState::PHYSICAL_KEYS KEYBOARD_BUTTONS_MAPPING[ NUM_WPF_KEYBOARD_BUTTONS ] =
{
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///Cancel
	KeyboardState::PHYSICAL_KEYS::KEY_BACK,
	KeyboardState::PHYSICAL_KEYS::KEY_TAB,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///LineFeed
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///Clear
	KeyboardState::PHYSICAL_KEYS::KEY_RETURN,
	KeyboardState::PHYSICAL_KEYS::KEY_PAUSE,
	KeyboardState::PHYSICAL_KEYS::KEY_CAPSLOCK,
	KeyboardState::PHYSICAL_KEYS::KEY_KANA,		///KanaMode
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///JunjaMode
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///FinalMode
	KeyboardState::PHYSICAL_KEYS::KEY_KANJI,
	KeyboardState::PHYSICAL_KEYS::KEY_ESCAPE,
	KeyboardState::PHYSICAL_KEYS::KEY_CONVERT,
	KeyboardState::PHYSICAL_KEYS::KEY_NOCONVERT,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///ImeAccept
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///ImeModeChange
	KeyboardState::PHYSICAL_KEYS::KEY_SPACE,
	KeyboardState::PHYSICAL_KEYS::KEY_PGUP,
	KeyboardState::PHYSICAL_KEYS::KEY_PGDN,
	KeyboardState::PHYSICAL_KEYS::KEY_END,
	KeyboardState::PHYSICAL_KEYS::KEY_HOME,
	KeyboardState::PHYSICAL_KEYS::KEY_LEFT,
	KeyboardState::PHYSICAL_KEYS::KEY_UP,
	KeyboardState::PHYSICAL_KEYS::KEY_RIGHT,
	KeyboardState::PHYSICAL_KEYS::KEY_DOWN,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///Select
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///Print
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///Execute
	KeyboardState::PHYSICAL_KEYS::KEY_SYSRQ,	///PrintScreen
	KeyboardState::PHYSICAL_KEYS::KEY_INSERT,
	KeyboardState::PHYSICAL_KEYS::KEY_DELETE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///Help
	KeyboardState::PHYSICAL_KEYS::KEY_0,
	KeyboardState::PHYSICAL_KEYS::KEY_1,
	KeyboardState::PHYSICAL_KEYS::KEY_2,
	KeyboardState::PHYSICAL_KEYS::KEY_3,
	KeyboardState::PHYSICAL_KEYS::KEY_4,
	KeyboardState::PHYSICAL_KEYS::KEY_5,
	KeyboardState::PHYSICAL_KEYS::KEY_6,
	KeyboardState::PHYSICAL_KEYS::KEY_7,
	KeyboardState::PHYSICAL_KEYS::KEY_8,
	KeyboardState::PHYSICAL_KEYS::KEY_9,
	KeyboardState::PHYSICAL_KEYS::KEY_A,
	KeyboardState::PHYSICAL_KEYS::KEY_B,
	KeyboardState::PHYSICAL_KEYS::KEY_C,
	KeyboardState::PHYSICAL_KEYS::KEY_D,
	KeyboardState::PHYSICAL_KEYS::KEY_E,
	KeyboardState::PHYSICAL_KEYS::KEY_F,
	KeyboardState::PHYSICAL_KEYS::KEY_G,
	KeyboardState::PHYSICAL_KEYS::KEY_H,
	KeyboardState::PHYSICAL_KEYS::KEY_I,
	KeyboardState::PHYSICAL_KEYS::KEY_J,
	KeyboardState::PHYSICAL_KEYS::KEY_K,
	KeyboardState::PHYSICAL_KEYS::KEY_L,
	KeyboardState::PHYSICAL_KEYS::KEY_M,
	KeyboardState::PHYSICAL_KEYS::KEY_N,
	KeyboardState::PHYSICAL_KEYS::KEY_O,
	KeyboardState::PHYSICAL_KEYS::KEY_P,
	KeyboardState::PHYSICAL_KEYS::KEY_Q,
	KeyboardState::PHYSICAL_KEYS::KEY_R,
	KeyboardState::PHYSICAL_KEYS::KEY_S,
	KeyboardState::PHYSICAL_KEYS::KEY_T,
	KeyboardState::PHYSICAL_KEYS::KEY_U,
	KeyboardState::PHYSICAL_KEYS::KEY_V,
	KeyboardState::PHYSICAL_KEYS::KEY_W,
	KeyboardState::PHYSICAL_KEYS::KEY_X,
	KeyboardState::PHYSICAL_KEYS::KEY_Y,
	KeyboardState::PHYSICAL_KEYS::KEY_Z,
	KeyboardState::PHYSICAL_KEYS::KEY_LWIN,
	KeyboardState::PHYSICAL_KEYS::KEY_RWIN,
	KeyboardState::PHYSICAL_KEYS::KEY_APPS,
	KeyboardState::PHYSICAL_KEYS::KEY_SLEEP,
	KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD0,
	KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD1,
	KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD2,
	KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD3,
	KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD4,
	KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD5,
	KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD6,
	KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD7,
	KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD8,
	KeyboardState::PHYSICAL_KEYS::KEY_NUMPAD9,
	KeyboardState::PHYSICAL_KEYS::KEY_MULTIPLY,
	KeyboardState::PHYSICAL_KEYS::KEY_ADD,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///Separator
	KeyboardState::PHYSICAL_KEYS::KEY_SUBTRACT,
	KeyboardState::PHYSICAL_KEYS::KEY_DECIMAL,
	KeyboardState::PHYSICAL_KEYS::KEY_DIVIDE,
	KeyboardState::PHYSICAL_KEYS::KEY_F1,
	KeyboardState::PHYSICAL_KEYS::KEY_F2,
	KeyboardState::PHYSICAL_KEYS::KEY_F3,
	KeyboardState::PHYSICAL_KEYS::KEY_F4,
	KeyboardState::PHYSICAL_KEYS::KEY_F5,
	KeyboardState::PHYSICAL_KEYS::KEY_F6,
	KeyboardState::PHYSICAL_KEYS::KEY_F7,
	KeyboardState::PHYSICAL_KEYS::KEY_F8,
	KeyboardState::PHYSICAL_KEYS::KEY_F9,
	KeyboardState::PHYSICAL_KEYS::KEY_F10,
	KeyboardState::PHYSICAL_KEYS::KEY_F11,
	KeyboardState::PHYSICAL_KEYS::KEY_F12,
	KeyboardState::PHYSICAL_KEYS::KEY_F13,
	KeyboardState::PHYSICAL_KEYS::KEY_F14,
	KeyboardState::PHYSICAL_KEYS::KEY_F15,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///F16
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///F24
	KeyboardState::PHYSICAL_KEYS::KEY_NUMLOCK,
	KeyboardState::PHYSICAL_KEYS::KEY_SCROLL,
	KeyboardState::PHYSICAL_KEYS::KEY_LSHIFT,
	KeyboardState::PHYSICAL_KEYS::KEY_RSHIFT,
	KeyboardState::PHYSICAL_KEYS::KEY_LCONTROL,
	KeyboardState::PHYSICAL_KEYS::KEY_RCONTROL,
	KeyboardState::PHYSICAL_KEYS::KEY_LALT,
	KeyboardState::PHYSICAL_KEYS::KEY_RALT,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///BrowserBack
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///BrowserForward
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///BrowserRefresh
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///BrowserStop
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///BrowserSearch
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///BrowserFavorites
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///BrowserHome
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///VolumeMute
	KeyboardState::PHYSICAL_KEYS::KEY_VOLUMEDOWN,
	KeyboardState::PHYSICAL_KEYS::KEY_VOLUMEUP,
	KeyboardState::PHYSICAL_KEYS::KEY_NEXTTRACK,
	KeyboardState::PHYSICAL_KEYS::KEY_PREVTRACK,
	KeyboardState::PHYSICAL_KEYS::KEY_MEDIASTOP,
	KeyboardState::PHYSICAL_KEYS::KEY_PLAYPAUSE,
	KeyboardState::PHYSICAL_KEYS::KEY_MAIL,
	KeyboardState::PHYSICAL_KEYS::KEY_MEDIASELECT,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///LaunchApplication1
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///LaunchApplication2
	KeyboardState::PHYSICAL_KEYS::KEY_SEMICOLON,
	KeyboardState::PHYSICAL_KEYS::KEY_ADD,		///OEM Plus
	KeyboardState::PHYSICAL_KEYS::KEY_COMMA,
	KeyboardState::PHYSICAL_KEYS::KEY_SUBTRACT,
	KeyboardState::PHYSICAL_KEYS::KEY_PERIOD,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///OEM Question
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///OEM tilde
	KeyboardState::PHYSICAL_KEYS::KEY_ABNT_C1,
	KeyboardState::PHYSICAL_KEYS::KEY_ABNT_C2,
	KeyboardState::PHYSICAL_KEYS::KEY_LBRACKET,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///OEM pipe
	KeyboardState::PHYSICAL_KEYS::KEY_RBRACKET,
	KeyboardState::PHYSICAL_KEYS::KEY_APOSTROPHE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///OEM8
	KeyboardState::PHYSICAL_KEYS::KEY_BACKSLASH,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///Ime Processed
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///System
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///OEM Attn
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///OEM finish
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///OEM Copy
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///OEM Auto
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///OEM Enlv
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///OEM back tab
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,		///OEM attn
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE,
	KeyboardState::PHYSICAL_KEYS::KEY_NONE
};


/**@brief Ustawia nowy stan przycisku na klawiaturze.

Wpf mo¿e wygenerowaæ eventy o kolejnych wciœniêciach kiedy u¿ytkownik wciœn¹³
i przytrzyma³ przez jakiœ czas klawisz. W takiej sytuacji te eventy s¹ równie¿
dostêpne.*/
void WPFInputProxy::KeyboardChange( int keyId, bool pressed )
{
	auto& keyboard = m_keyboards[ 0 ];
	auto state = keyboard->KeysState();

	// Nie u¿ywamy przeci¹¿onego operatora=, poniewa¿ wtedy nie moglibyœmy wy³apywaæ
	// wielokrotnych wciœnieæ (keystrokes??)
	if( pressed )
		state[ KEYBOARD_BUTTONS_MAPPING[ keyId ] ].Press();
	else
		state[ KEYBOARD_BUTTONS_MAPPING[ keyId ] ].UnPress();
}

/**@brief Ustawia nowy stan przycisku myszy.

@todo W przysz³oœci mo¿e trzeba bêdzie dodaæ informacjê o zmienie stanu.
Móg³by to byæ np któryæ bit ustawiony na 1 czy coœ.*/
void WPFInputProxy::MouseButtonChange( int button, bool pressed )
{
	auto& mouse = m_mouses[ 0 ];
	auto buttonsState = mouse->GetButtonsState();
	if( pressed )
		buttonsState[ MOUSE_BUTTONS_MAPPING[ button ] ].Press();
	else
		buttonsState[ MOUSE_BUTTONS_MAPPING[ button ] ].UnPress();
}

/**@brief Ustawia now¹ pozycjê myszy.*/
void WPFInputProxy::MousePositionChange( double X, double Y )
{
	auto& mouse = m_mouses[ 0 ];
	mouse->SetPosition( (short)X, (short)Y );
}

/**@brief Ustawia przesuniêcie kó³ka myszy.*/
void WPFInputProxy::MouseWheelChange( double delta )
{
	auto& mouse = m_mouses[ 0 ];
	mouse->GetAxesState()[ MouseState::PHYSICAL_AXES::WHEEL ] = (float)delta;
}

/**@brief Poniewa¿ okno straci³o focus to czyœcimy stan przycisków i myszy.

@todo Dokoñczyæ*/
void WPFInputProxy::LostFocus()
{

}

/**@brief Funkcja powinna zostaæ wywo³ana po zakoñczeniu przetwarzania inputu przez aplikacjê.*/
void WPFInputProxy::PostUpdate()
{
	MouseWheelChange( 0.0 );

	for( auto& keyboard : m_keyboards )
		keyboard->RemoveEvents();

	for( auto& mouse : m_mouses )
		mouse->RemoveEvents();
}


