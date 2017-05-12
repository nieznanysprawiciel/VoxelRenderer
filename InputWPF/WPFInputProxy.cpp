/**
@file WPFInputProxy.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "WPFInputProxy.h"




namespace sw {
namespace input
{




/**@brief */
WPFInputProxy::WPFInputProxy()
	:	m_lastX( 0 )
	,	m_lastY( 0 )
	,	m_eventNum( 0 )
{}

/**@brief */
WPFInputProxy::~WPFInputProxy()
{}

/**@brief */
bool			WPFInputProxy::Init( const InputInitInfo & initInfo )
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
const std::vector< const KeyboardState* >&	WPFInputProxy::GetKeyboardStates	() const
{
	return m_keyboardsStates;
}

/**@copydoc IInput::GetMouseStates*/
const std::vector< const MouseState* >&		WPFInputProxy::GetMouseStates	() const
{
	return m_mousesStates;
}

/**@copydoc IInput::GetJoystickStates*/
const std::vector< const JoystickState* >&	WPFInputProxy::GetJoystickStates	() const
{
	return m_joysticksStates;
}

/**@copydoc IInput::GetKeyboardDevice*/
std::vector< KeyboardDeviceOPtr >&		WPFInputProxy::GetKeyboardDevice()
{
	return m_keyboards;
}

/**@copydoc IInput::GetMouseDevice*/
std::vector< MouseDeviceOPtr >&			WPFInputProxy::GetMouseDevice()
{
	return m_mouses;
}

/**@copydoc IInput::GetJoystickDevice*/
std::vector< JoystickDeviceOPtr >&		WPFInputProxy::GetJoystickDevice()
{
	return m_joysticks;
}


/**@brief */
std::vector< const InputDeviceInfo* >	WPFInputProxy::GetDevicesInfo		() const
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
	m_eventNum = 0;

	//auto& mouse = m_mouses[ 0 ];
	//auto axes = mouse->GetAxesState();

	//axes[ Mouse::PhysicalAxes::X_AXIS ] = static_cast<float>( mouse->GetPositionX() - m_lastX );
	//axes[ Mouse::PhysicalAxes::Y_AXIS ] = static_cast<float>( mouse->GetPositionY() - m_lastY );

	//m_lastX = mouse->GetPositionX();
	//m_lastY = mouse->GetPositionY();
}

/**@copydoc IInput::UpdateDevices

Urz¹dzenie jest zawsze aktualne.*/
bool WPFInputProxy::UpdateDevices()
{
	return true;
}

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
Mouse::PhysicalButtons MOUSE_BUTTONS_MAPPING[ NUM_WPF_MOUSE_BUTTONS ] =
{
	Mouse::PhysicalButtons::LEFT_BUTTON,
	Mouse::PhysicalButtons::MIDDLE_BUTTON,
	Mouse::PhysicalButtons::RIGHT_BUTTON,
	Mouse::PhysicalButtons::BUTTON3,
	Mouse::PhysicalButtons::BUTTON4
};

#define NUM_WPF_KEYBOARD_BUTTONS 173

/**@brief Tablica mapowania przycisków klawiatury WPFa na wartoœci silnikowe.*/
Keyboard::PhysicalKeys KEYBOARD_BUTTONS_MAPPING[ NUM_WPF_KEYBOARD_BUTTONS ] =
{
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,		///Cancel
	Keyboard::PhysicalKeys::KEY_BACK,
	Keyboard::PhysicalKeys::KEY_TAB,
	Keyboard::PhysicalKeys::KEY_NONE,		///LineFeed
	Keyboard::PhysicalKeys::KEY_NONE,		///Clear
	Keyboard::PhysicalKeys::KEY_RETURN,
	Keyboard::PhysicalKeys::KEY_PAUSE,
	Keyboard::PhysicalKeys::KEY_CAPSLOCK,
	Keyboard::PhysicalKeys::KEY_KANA,		///KanaMode
	Keyboard::PhysicalKeys::KEY_NONE,		///JunjaMode
	Keyboard::PhysicalKeys::KEY_NONE,		///FinalMode
	Keyboard::PhysicalKeys::KEY_KANJI,
	Keyboard::PhysicalKeys::KEY_ESCAPE,
	Keyboard::PhysicalKeys::KEY_CONVERT,
	Keyboard::PhysicalKeys::KEY_NOCONVERT,
	Keyboard::PhysicalKeys::KEY_NONE,		///ImeAccept
	Keyboard::PhysicalKeys::KEY_NONE,		///ImeModeChange
	Keyboard::PhysicalKeys::KEY_SPACE,
	Keyboard::PhysicalKeys::KEY_PGUP,
	Keyboard::PhysicalKeys::KEY_PGDN,
	Keyboard::PhysicalKeys::KEY_END,
	Keyboard::PhysicalKeys::KEY_HOME,
	Keyboard::PhysicalKeys::KEY_LEFT,
	Keyboard::PhysicalKeys::KEY_UP,
	Keyboard::PhysicalKeys::KEY_RIGHT,
	Keyboard::PhysicalKeys::KEY_DOWN,
	Keyboard::PhysicalKeys::KEY_NONE,		///Select
	Keyboard::PhysicalKeys::KEY_NONE,		///Print
	Keyboard::PhysicalKeys::KEY_NONE,		///Execute
	Keyboard::PhysicalKeys::KEY_SYSRQ,	///PrintScreen
	Keyboard::PhysicalKeys::KEY_INSERT,
	Keyboard::PhysicalKeys::KEY_DELETE,
	Keyboard::PhysicalKeys::KEY_NONE,		///Help
	Keyboard::PhysicalKeys::KEY_0,
	Keyboard::PhysicalKeys::KEY_1,
	Keyboard::PhysicalKeys::KEY_2,
	Keyboard::PhysicalKeys::KEY_3,
	Keyboard::PhysicalKeys::KEY_4,
	Keyboard::PhysicalKeys::KEY_5,
	Keyboard::PhysicalKeys::KEY_6,
	Keyboard::PhysicalKeys::KEY_7,
	Keyboard::PhysicalKeys::KEY_8,
	Keyboard::PhysicalKeys::KEY_9,
	Keyboard::PhysicalKeys::KEY_A,
	Keyboard::PhysicalKeys::KEY_B,
	Keyboard::PhysicalKeys::KEY_C,
	Keyboard::PhysicalKeys::KEY_D,
	Keyboard::PhysicalKeys::KEY_E,
	Keyboard::PhysicalKeys::KEY_F,
	Keyboard::PhysicalKeys::KEY_G,
	Keyboard::PhysicalKeys::KEY_H,
	Keyboard::PhysicalKeys::KEY_I,
	Keyboard::PhysicalKeys::KEY_J,
	Keyboard::PhysicalKeys::KEY_K,
	Keyboard::PhysicalKeys::KEY_L,
	Keyboard::PhysicalKeys::KEY_M,
	Keyboard::PhysicalKeys::KEY_N,
	Keyboard::PhysicalKeys::KEY_O,
	Keyboard::PhysicalKeys::KEY_P,
	Keyboard::PhysicalKeys::KEY_Q,
	Keyboard::PhysicalKeys::KEY_R,
	Keyboard::PhysicalKeys::KEY_S,
	Keyboard::PhysicalKeys::KEY_T,
	Keyboard::PhysicalKeys::KEY_U,
	Keyboard::PhysicalKeys::KEY_V,
	Keyboard::PhysicalKeys::KEY_W,
	Keyboard::PhysicalKeys::KEY_X,
	Keyboard::PhysicalKeys::KEY_Y,
	Keyboard::PhysicalKeys::KEY_Z,
	Keyboard::PhysicalKeys::KEY_LWIN,
	Keyboard::PhysicalKeys::KEY_RWIN,
	Keyboard::PhysicalKeys::KEY_APPS,
	Keyboard::PhysicalKeys::KEY_SLEEP,
	Keyboard::PhysicalKeys::KEY_NUMPAD0,
	Keyboard::PhysicalKeys::KEY_NUMPAD1,
	Keyboard::PhysicalKeys::KEY_NUMPAD2,
	Keyboard::PhysicalKeys::KEY_NUMPAD3,
	Keyboard::PhysicalKeys::KEY_NUMPAD4,
	Keyboard::PhysicalKeys::KEY_NUMPAD5,
	Keyboard::PhysicalKeys::KEY_NUMPAD6,
	Keyboard::PhysicalKeys::KEY_NUMPAD7,
	Keyboard::PhysicalKeys::KEY_NUMPAD8,
	Keyboard::PhysicalKeys::KEY_NUMPAD9,
	Keyboard::PhysicalKeys::KEY_MULTIPLY,
	Keyboard::PhysicalKeys::KEY_ADD,
	Keyboard::PhysicalKeys::KEY_NONE,		///Separator
	Keyboard::PhysicalKeys::KEY_SUBTRACT,
	Keyboard::PhysicalKeys::KEY_DECIMAL,
	Keyboard::PhysicalKeys::KEY_DIVIDE,
	Keyboard::PhysicalKeys::KEY_F1,
	Keyboard::PhysicalKeys::KEY_F2,
	Keyboard::PhysicalKeys::KEY_F3,
	Keyboard::PhysicalKeys::KEY_F4,
	Keyboard::PhysicalKeys::KEY_F5,
	Keyboard::PhysicalKeys::KEY_F6,
	Keyboard::PhysicalKeys::KEY_F7,
	Keyboard::PhysicalKeys::KEY_F8,
	Keyboard::PhysicalKeys::KEY_F9,
	Keyboard::PhysicalKeys::KEY_F10,
	Keyboard::PhysicalKeys::KEY_F11,
	Keyboard::PhysicalKeys::KEY_F12,
	Keyboard::PhysicalKeys::KEY_F13,
	Keyboard::PhysicalKeys::KEY_F14,
	Keyboard::PhysicalKeys::KEY_F15,
	Keyboard::PhysicalKeys::KEY_NONE,		///F16
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,		///F24
	Keyboard::PhysicalKeys::KEY_NUMLOCK,
	Keyboard::PhysicalKeys::KEY_SCROLL,
	Keyboard::PhysicalKeys::KEY_LSHIFT,
	Keyboard::PhysicalKeys::KEY_RSHIFT,
	Keyboard::PhysicalKeys::KEY_LCONTROL,
	Keyboard::PhysicalKeys::KEY_RCONTROL,
	Keyboard::PhysicalKeys::KEY_LALT,
	Keyboard::PhysicalKeys::KEY_RALT,
	Keyboard::PhysicalKeys::KEY_NONE,		///BrowserBack
	Keyboard::PhysicalKeys::KEY_NONE,		///BrowserForward
	Keyboard::PhysicalKeys::KEY_NONE,		///BrowserRefresh
	Keyboard::PhysicalKeys::KEY_NONE,		///BrowserStop
	Keyboard::PhysicalKeys::KEY_NONE,		///BrowserSearch
	Keyboard::PhysicalKeys::KEY_NONE,		///BrowserFavorites
	Keyboard::PhysicalKeys::KEY_NONE,		///BrowserHome
	Keyboard::PhysicalKeys::KEY_NONE,		///VolumeMute
	Keyboard::PhysicalKeys::KEY_VOLUMEDOWN,
	Keyboard::PhysicalKeys::KEY_VOLUMEUP,
	Keyboard::PhysicalKeys::KEY_NEXTTRACK,
	Keyboard::PhysicalKeys::KEY_PREVTRACK,
	Keyboard::PhysicalKeys::KEY_MEDIASTOP,
	Keyboard::PhysicalKeys::KEY_PLAYPAUSE,
	Keyboard::PhysicalKeys::KEY_MAIL,
	Keyboard::PhysicalKeys::KEY_MEDIASELECT,
	Keyboard::PhysicalKeys::KEY_NONE,		///LaunchApplication1
	Keyboard::PhysicalKeys::KEY_NONE,		///LaunchApplication2
	Keyboard::PhysicalKeys::KEY_SEMICOLON,
	Keyboard::PhysicalKeys::KEY_ADD,		///OEM Plus
	Keyboard::PhysicalKeys::KEY_COMMA,
	Keyboard::PhysicalKeys::KEY_SUBTRACT,
	Keyboard::PhysicalKeys::KEY_PERIOD,
	Keyboard::PhysicalKeys::KEY_NONE,		///OEM Question
	Keyboard::PhysicalKeys::KEY_NONE,		///OEM tilde
	Keyboard::PhysicalKeys::KEY_ABNT_C1,
	Keyboard::PhysicalKeys::KEY_ABNT_C2,
	Keyboard::PhysicalKeys::KEY_LBRACKET,
	Keyboard::PhysicalKeys::KEY_NONE,		///OEM pipe
	Keyboard::PhysicalKeys::KEY_RBRACKET,
	Keyboard::PhysicalKeys::KEY_APOSTROPHE,
	Keyboard::PhysicalKeys::KEY_NONE,		///OEM8
	Keyboard::PhysicalKeys::KEY_BACKSLASH,
	Keyboard::PhysicalKeys::KEY_NONE,		///Ime Processed
	Keyboard::PhysicalKeys::KEY_NONE,		///System
	Keyboard::PhysicalKeys::KEY_NONE,		///OEM Attn
	Keyboard::PhysicalKeys::KEY_NONE,		///OEM finish
	Keyboard::PhysicalKeys::KEY_NONE,		///OEM Copy
	Keyboard::PhysicalKeys::KEY_NONE,		///OEM Auto
	Keyboard::PhysicalKeys::KEY_NONE,		///OEM Enlv
	Keyboard::PhysicalKeys::KEY_NONE,		///OEM back tab
	Keyboard::PhysicalKeys::KEY_NONE,		///OEM attn
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE,
	Keyboard::PhysicalKeys::KEY_NONE
};


/**@brief Ustawia nowy stan przycisku na klawiaturze.

Wpf mo¿e wygenerowaæ eventy o kolejnych wciœniêciach kiedy u¿ytkownik wciœn¹³
i przytrzyma³ przez jakiœ czas klawisz. W takiej sytuacji te eventy s¹ równie¿
dostêpne.*/
void			WPFInputProxy::KeyboardChange		( int keyId, bool pressed )
{
	//auto& keyboard = m_keyboards[ 0 ];
	//auto state = keyboard->KeysState();

	//// Nie u¿ywamy przeci¹¿onego operatora=, poniewa¿ wtedy nie moglibyœmy wy³apywaæ
	//// wielokrotnych wciœnieæ (keystrokes??)
	//if( pressed )
	//	state[ KEYBOARD_BUTTONS_MAPPING[ keyId ] ].Press();
	//else
	//	state[ KEYBOARD_BUTTONS_MAPPING[ keyId ] ].UnPress();

	KeyEvent keyEvent;
	keyEvent.Key = KEYBOARD_BUTTONS_MAPPING[ keyId ];
	keyEvent.State = pressed;

	m_keyboards[ 0 ]->AddEvent( DeviceEvent( keyEvent, m_eventNum++ ) );
}

/**@brief Ustawia nowy stan przycisku myszy.

@todo W przysz³oœci mo¿e trzeba bêdzie dodaæ informacjê o zmienie stanu.
Móg³by to byæ np któryæ bit ustawiony na 1 czy coœ.*/
void			WPFInputProxy::MouseButtonChange		( int button, bool pressed )
{
	//auto& mouse = m_mouses[ 0 ];
	//auto buttonsState = mouse->GetButtonsState();
	//if( pressed )
	//	buttonsState[ MOUSE_BUTTONS_MAPPING[ button ] ].Press();
	//else
	//	buttonsState[ MOUSE_BUTTONS_MAPPING[ button ] ].UnPress();

	ButtonEvent mouseButtonEvt;
	mouseButtonEvt.Button = MOUSE_BUTTONS_MAPPING[ button ];
	mouseButtonEvt.State = pressed;

	m_mouses[ 0 ]->AddEvent( DeviceEvent( mouseButtonEvt, m_eventNum++ )  );
}

/**@brief Ustawia now¹ pozycjê myszy.*/
void			WPFInputProxy::MousePositionChange		( double X, double Y )
{
	//auto& mouse = m_mouses[ 0 ];
	//mouse->SetPosition( (short)X, (short)Y );

	auto& mouse = m_mousesStates[ 0 ];
	
	CursorEvent cursorEvent;
	cursorEvent.OffsetX = (uint16)X - m_lastX;
	cursorEvent.OffsetY = (uint16)Y - m_lastY;

	m_mouses[ 0 ]->AddEvent( DeviceEvent( cursorEvent, m_eventNum++ ) );

	m_lastX = (uint16)X;
	m_lastY = (uint16)Y;

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
void			WPFInputProxy::MouseWheelChange			( double delta )
{
	//auto& mouse = m_mouses[ 0 ];
	//mouse->GetAxesState()[ Mouse::PhysicalAxes::WHEEL ] = (float)delta;

	AxisEvent wheel;
	wheel.Delta = (float)delta;
	wheel.Axis = Mouse::PhysicalAxes::WHEEL;

	m_mouses[ 0 ]->AddEvent( DeviceEvent( wheel, m_eventNum++ ) );
}

/**@brief Poniewa¿ okno straci³o focus to czyœcimy stan przycisków i myszy.

@todo Dokoñczyæ*/
void			WPFInputProxy::LostFocus()
{

}

/**@brief Funkcja powinna zostaæ wywo³ana po zakoñczeniu przetwarzania inputu przez aplikacjê.*/
void			WPFInputProxy::PostUpdate()
{
	//MouseWheelChange( 0.0 );

	auto& mouse = m_mouses[ 0 ];
	
	// Zero wheel.
	AxisEvent wheel;
	wheel.Delta = (float)-mouse->GetState().GetAxesState()[ Mouse::PhysicalAxes::WHEEL ];
	wheel.Axis = Mouse::PhysicalAxes::WHEEL;

	m_mouses[ 0 ]->AddEvent( DeviceEvent( wheel, m_eventNum++ ) );

	//for( auto& keyboard : m_keyboards )
	//	keyboard->RemoveEvents();

	//for( auto& mouse : m_mouses )
	//	mouse->RemoveEvents();
}



}	// input
}	// sw
