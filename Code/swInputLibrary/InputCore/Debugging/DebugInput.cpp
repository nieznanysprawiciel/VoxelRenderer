/**
@file DebugInput.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "DebugInput.h"


namespace sw {
namespace input
{


// ================================ //
//
bool			DebugInput::Init		( const InputInitInfo& initInfo )
{
	m_keyboards.push_back( KeyboardDeviceOPtr( new KeyboardDevice() ) );
	m_mouses.push_back( MouseDeviceOPtr( new MouseDevice() ) );
	m_joysticks.push_back( JoystickDeviceOPtr( new JoystickDevice() ) );

	m_keyboardsStates.push_back( &m_keyboards[ 0 ]->GetState() );
	m_mousesStates.push_back( &m_mouses[ 0 ]->GetState() );
	m_joysticksStates.push_back( &m_joysticks[ 0 ]->GetState() );

	m_frameNumber = 0;
	m_eventCapture = initInfo.EventCapturer;


	return true;
}

/**@copydoc IInput::GetKeyboardStates*/
const std::vector< const KeyboardState* >&	DebugInput::GetKeyboardStates	() const
{
	return m_keyboardsStates;
}

/**@copydoc IInput::GetMouseStates*/
const std::vector< const MouseState* >&		DebugInput::GetMouseStates	() const
{
	return m_mousesStates;
}

/**@copydoc IInput::GetJoystickStates*/
const std::vector< const JoystickState* >&	DebugInput::GetJoystickStates	() const
{
	return m_joysticksStates;
}

/**@copydoc IInput::GetKeyboardDevice*/
std::vector< KeyboardDeviceOPtr >&		DebugInput::GetKeyboardDevice()
{
	return m_keyboards;
}

/**@copydoc IInput::GetMouseDevice*/
std::vector< MouseDeviceOPtr >&			DebugInput::GetMouseDevice()
{
	return m_mouses;
}

/**@copydoc IInput::GetJoystickDevice*/
std::vector< JoystickDeviceOPtr >&		DebugInput::GetJoystickDevice()
{
	return m_joysticks;
}

/**@brief */
std::vector< const InputDeviceInfo* >	DebugInput::GetDevicesInfo		() const
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

// ================================ //
//
void											DebugInput::Update				( float timeInterval )
{
	m_eventNum = 0;
	
	for( auto& keyboard : m_keyboards )
		keyboard->RemoveEvents();

	for( auto& mouse : m_mouses )
		mouse->RemoveEvents();

	for( auto& joystick : m_joysticks )
		joystick->RemoveEvents();

	while( m_eventCapture->IsNext( m_frameNumber ) )
	{
		auto& event = m_eventCapture->QueryEvent( m_frameNumber );
		switch( event.EventContent.Type )
		{
			case DeviceEventType::KeyboardEvent:
			case DeviceEventType::CharacterEvent:
				m_keyboards[ event.DeviceIdx ]->AddEvent( event.EventContent );
				break;
			case DeviceEventType::AxisEvent:
			case DeviceEventType::ButtonEvent:
			case DeviceEventType::CursorEvent:
				m_mouses[ event.DeviceIdx ]->AddEvent( event.EventContent );
				break;
			default:
				break;
		}
	}

	m_frameNumber++;
	m_eventCapture->GoToNextFrame();
}

// ================================ //
//
bool											DebugInput::UpdateDevices()
{
	return false;
}



}	// input
}	// sw
