#include "DirectInputModule.h"
#include "swCommonLib/Common/TypesDefinitions.h"

#include "swInputLibrary/Factory.h"

#include "swCommonLib/Common/MemoryLeaks.h"

#include <assert.h>


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
{
	ClearInputStates();
}

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

	m_keyboards.push_back( new KeyboardState() );
	m_mouses.push_back( new MouseState() );
	m_joysticks.push_back( new JoystickState() );

	return true;
}




/**@copydoc IInput::GetKeyboardStates*/
const std::vector< KeyboardState* >&	DirectInputModule::GetKeyboardStates	()
{
	return m_keyboards;
}

/**@copydoc IInput::GetMouseStates*/
const std::vector< MouseState* >&		DirectInputModule::GetMouseStates		()
{
	return m_mouses;
}

/**@copydoc IInput::GetJoystickStates*/
const std::vector< JoystickState* >&	DirectInputModule::GetJoystickStates	()
{
	return m_joysticks;
}

/**@brief */
std::vector< const InputDeviceInfo* >	DirectInputModule::GetDevicesInfo		()
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
	
	result = GetCursorPos( &pos ) !=0;
	result = result && ScreenToClient( m_windowHandle, &pos ) !=0;
	assert( result );

	for( Size i = 0; i < m_mouses.size(); ++i )
	{
		UpdateMouse( (int)i );
		m_mouses[ i ]->SetPosition( (short)pos.x, (short)pos.y );
	}

	// Joystick
	for( Size i = 0; i < m_joysticks.size(); ++i )
		UpdateJoystick( (int)i );
}

/**@brief */
void DirectInputModule::UpdateKeyboard( int idx )
{
	char newState[ KEYBOARD_STATE_KEYS_NUMBER ];
	m_keyboardInput->GetDeviceState( KEYBOARD_STATE_KEYS_NUMBER, newState );

	auto prevState = m_keyboards[ idx ]->KeysState();
	for( int i = 0; i < KEYBOARD_STATE_KEYS_NUMBER; ++i )
		prevState[ i ] = newState[ i ] != 0;
}

/**@brief */
void DirectInputModule::UpdateMouse( int idx )
{
	DIMOUSESTATE2	mouseStruct;
	m_mouseInput->GetDeviceState( sizeof( mouseStruct ), &mouseStruct );
	
	auto mouseAxes = m_mouses[ idx ]->GetAxesState();
	mouseAxes[ 0 ] = static_cast< float >( mouseStruct.lX );
	mouseAxes[ 1 ] = static_cast< float >( mouseStruct.lY );
	mouseAxes[ 2 ] = static_cast< float >( mouseStruct.lZ );

	auto mouseButtons = m_mouses[ idx ]->GetButtonsState();
	for( int i = 0; i < 8; ++i )
		mouseButtons[ i ] = mouseStruct.rgbButtons[ i ] != 0;
}

/**@brief */
void DirectInputModule::UpdateJoystick( int idx )
{ }


/**@brief */
bool									DirectInputModule::UpdateDevices()
{
	assert( !"Zaimplementuj mnie" );
	return false;
}

/**@brief Zwalania zasoby DirectInputa.*/
void DirectInputModule::CleanDirectInput()
{
	if ( m_directInput != nullptr )
	{
		if ( m_keyboardInput != nullptr )
		{
			m_keyboardInput->Unacquire();
			m_keyboardInput->Release();
		}

		if ( m_mouseInput != nullptr)
		{
			m_mouseInput->Unacquire();
			m_mouseInput->Release();
		}
		m_directInput->Release();
	}
}

/**@brief */
void			DirectInputModule::ClearInputStates	()
{
	for( auto state : m_keyboards )
		delete state;
	for( auto state : m_mouses )
		delete state;
	for( auto state : m_joysticks )
		delete state;
}

//====================================================================================//
//			Creating	
//====================================================================================//

IInput*			InputFactory::CreateDirectInput		()
{
	return new DirectInputModule();
}
