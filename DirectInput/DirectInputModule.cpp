#include "DirectInputModule.h"
#include "swCommonLib/Common/TypesDefinitions.h"

#include "swInputLibrary/InputCore/Factory.h"

#include "swCommonLib/Common/MemoryLeaks.h"

#include <assert.h>


namespace sw {
namespace input
{



#define DIRECT_INPUT_OK				DI_OK

/**@brief */
DirectInputModule::DirectInputModule()
{
	m_windowHandle = nullptr;

	m_directInput = nullptr;
	m_mouseInput = nullptr;
	m_keyboardInput = nullptr;
}


/**@brief */
DirectInputModule::~DirectInputModule()
{}

/**@brief */
bool									DirectInputModule::Init				( const InputInitInfo& initInfo )
{
	m_windowHandle = (HWND)initInfo.WndHandle;
	int result = DirectInput8Create( (HINSTANCE)initInfo.AppInstance,
									 DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_directInput, NULL );
	if( result != DIRECT_INPUT_OK )
		return false;

	//tworzymy obiekt klawiatury
	result = m_directInput->CreateDevice( GUID_SysKeyboard, &m_keyboardInput, nullptr );
	if( result != DIRECT_INPUT_OK )
	{
		m_directInput->Release();
		return false;
	}
	m_keyboardInput->SetDataFormat( &c_dfDIKeyboard );
	m_keyboardInput->SetCooperativeLevel( (HWND)initInfo.WndHandle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE );

	//tworzymy obiekt myszy
	result = m_directInput->CreateDevice( GUID_SysMouse, &m_mouseInput, nullptr );
	if( result != DIRECT_INPUT_OK )
	{
		m_keyboardInput->Release();
		m_directInput->Release();
		return false;
	}
	m_mouseInput->SetDataFormat( &c_dfDIMouse2 );
	m_mouseInput->SetCooperativeLevel( (HWND)initInfo.WndHandle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE );

	//todo: sprawdziæ czy jest w systemie joystick i go wczytaæ

	m_keyboardInput->Acquire();
	m_mouseInput->Acquire();

	m_keyboards.push_back( KeyboardDeviceOPtr( new KeyboardDevice() ) );
	m_mouses.push_back( MouseDeviceOPtr( new MouseDevice() ) );
	m_joysticks.push_back( JoystickDeviceOPtr( new JoystickDevice() ) );

	m_keyboardsStates.push_back( &m_keyboards[ 0 ]->GetState() );
	m_mousesStates.push_back( &m_mouses[ 0 ]->GetState() );
	m_joysticksStates.push_back( &m_joysticks[ 0 ]->GetState() );


	return true;
}


/**@copydoc IInput::GetKeyboardStates*/
const std::vector< const KeyboardState* >&	DirectInputModule::GetKeyboardStates	() const
{
	return m_keyboardsStates;
}

/**@copydoc IInput::GetMouseStates*/
const std::vector< const MouseState* >&		DirectInputModule::GetMouseStates	() const
{
	return m_mousesStates;
}

/**@copydoc IInput::GetJoystickStates*/
const std::vector< const JoystickState* >&	DirectInputModule::GetJoystickStates	() const
{
	return m_joysticksStates;
}

/**@copydoc IInput::GetKeyboardDevice*/
std::vector< KeyboardDeviceOPtr >&		DirectInputModule::GetKeyboardDevice()
{
	return m_keyboards;
}

/**@copydoc IInput::GetMouseDevice*/
std::vector< MouseDeviceOPtr >&			DirectInputModule::GetMouseDevice()
{
	return m_mouses;
}

/**@copydoc IInput::GetJoystickDevice*/
std::vector< JoystickDeviceOPtr >&		DirectInputModule::GetJoystickDevice()
{
	return m_joysticks;
}

/**@brief */
std::vector< const InputDeviceInfo* >	DirectInputModule::GetDevicesInfo		() const
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

/**@brief */
void									DirectInputModule::Update				( float timeInterval )
{
	// Klawiatura
	for( Size i = 0; i < m_keyboards.size(); ++i )
		UpdateKeyboard( (int)i );

	// Myszka
	bool result;
	POINT pos;

	result = GetCursorPos( &pos ) != 0;
	result = result && ScreenToClient( m_windowHandle, &pos ) != 0;
	assert( result );

	//for( Size i = 0; i < m_mouses.size(); ++i )
	//{
	//	UpdateMouse( (int)i );
	//	m_mouses[ i ]->SetPosition( (short)pos.x, (short)pos.y );
	//}

	assert( false );
	///@todo Repair

	// Joystick
	for( Size i = 0; i < m_joysticks.size(); ++i )
		UpdateJoystick( (int)i );
}

/**@brief */
void				DirectInputModule::UpdateKeyboard( int idx )
{
	//char newState[ KEYBOARD_STATE_KEYS_NUMBER ];
	//m_keyboardInput->GetDeviceState( KEYBOARD_STATE_KEYS_NUMBER, newState );

	//auto prevState = m_keyboards[ idx ]->KeysState();
	//for( int i = 0; i < KEYBOARD_STATE_KEYS_NUMBER; ++i )
	//	prevState[ i ] = newState[ i ] != 0;

	assert( false );
	///@todo Repair
}

/**@brief */
void				DirectInputModule::UpdateMouse( int idx )
{
	//DIMOUSESTATE2	mouseStruct;
	//m_mouseInput->GetDeviceState( sizeof( mouseStruct ), &mouseStruct );

	//auto mouseAxes = m_mouses[ idx ]->GetAxesState();
	//mouseAxes[ 0 ] = static_cast<float>( mouseStruct.lX );
	//mouseAxes[ 1 ] = static_cast<float>( mouseStruct.lY );
	//mouseAxes[ 2 ] = static_cast<float>( mouseStruct.lZ );

	//auto mouseButtons = m_mouses[ idx ]->GetButtonsState();
	//for( int i = 0; i < 8; ++i )
	//	mouseButtons[ i ] = mouseStruct.rgbButtons[ i ] != 0;

	assert( false );
	///@todo Repair
}

/**@brief */
void				DirectInputModule::UpdateJoystick( int idx )
{}


/**@brief */
bool									DirectInputModule::UpdateDevices()
{
	assert( !"Zaimplementuj mnie" );
	return false;
}

/**@brief Zwalania zasoby DirectInputa.*/
void DirectInputModule::CleanDirectInput()
{
	if( m_directInput != nullptr )
	{
		if( m_keyboardInput != nullptr )
		{
			m_keyboardInput->Unacquire();
			m_keyboardInput->Release();
		}

		if( m_mouseInput != nullptr )
		{
			m_mouseInput->Unacquire();
			m_mouseInput->Release();
		}
		m_directInput->Release();
	}
}

//====================================================================================//
//			Creating	
//====================================================================================//

IInput*			InputFactory::CreateDirectInput		()
{
	return new DirectInputModule();
}


}	// input
}	// sw

