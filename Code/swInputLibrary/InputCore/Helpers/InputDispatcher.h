#pragma once
/**
@file InputDispatcher.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swInputLibrary/InputCore/IInput.h"
#include "swInputLibrary/InputCore/Device.h"


namespace sw {
namespace input
{

/**@brief Provides input events in order of creation.
This class uses timestamp to sort events.*/
class InputDispatcher
{
public:

	struct DispatchedEvent
	{
		DeviceEvent		Event;
		Device*			ProducerDevice;

	// ================================ //
	//
		DispatchedEvent( Device* device, DeviceEvent evt )
			:	Event( evt )
			,	ProducerDevice( device )
		{}
	};

private:

	std::vector< KeyboardDeviceOPtr >&		m_keyboards;
	std::vector< MouseDeviceOPtr >&			m_mouses;
	std::vector< JoystickDeviceOPtr >&		m_joysticks;

	Timestamp			m_nextTimestamp;
	Size				m_eventLeft;

protected:
public:
	explicit		InputDispatcher		( IInput* input );
					~InputDispatcher	() = default;


	DispatchedEvent			NextEvent		();
	bool					NoEventsLeft	();


	std::vector< KeyboardDeviceOPtr >&		GetKeyboards	() { return m_keyboards; }
	std::vector< MouseDeviceOPtr >&			GetMouses		() { return m_mouses; }
	std::vector< JoystickDeviceOPtr >&		GetJoysticks	() { return m_joysticks; }

private:

	DispatchedEvent			FindEvent			( Timestamp timestamp );
	Timestamp				FindMinTimestamp	();
};


//====================================================================================//
//			Inline implementation	
//====================================================================================//


inline		InputDispatcher::InputDispatcher		( IInput* input )
	:	m_keyboards( input->GetKeyboardDevice() )
	,	m_mouses( input->GetMouseDevice() )
	,	m_joysticks( input->GetJoystickDevice() )
	,	m_nextTimestamp( 0 )
	,	m_eventLeft( 0 )
{
	for( auto & mouse : m_mouses )
		m_eventLeft += mouse->GetEventsQueue().EventsLeft();

	for( auto & keyboard : m_keyboards )
		m_eventLeft += keyboard->GetEventsQueue().EventsLeft();

	for( auto & joystick : m_joysticks )
		m_eventLeft += joystick->GetEventsQueue().EventsLeft();
}

// ================================ //
//
inline InputDispatcher::DispatchedEvent		InputDispatcher::NextEvent()
{
	if( NoEventsLeft() )
		return InputDispatcher::DispatchedEvent( nullptr, DeviceEvent() );

	m_eventLeft--;

	auto dispatched = FindEvent( m_nextTimestamp );
	if( dispatched.ProducerDevice == nullptr )
	{
		// Only informational assert. You can remove in future.
		assert( !"Lost event ID" );

		// If event with m_nextTimestamp not found.
		m_nextTimestamp = FindMinTimestamp();
		return FindEvent( m_nextTimestamp++ );
	}

	m_nextTimestamp++;
	return dispatched;
}

// ================================ //
//
inline bool		InputDispatcher::NoEventsLeft()
{
	return m_eventLeft == 0;
}

// ================================ //
//
inline InputDispatcher::DispatchedEvent		InputDispatcher::FindEvent		( Timestamp timestamp )
{
	// Mouse events are will be the most often.
	for( int i = 0; i < m_mouses.size(); i++ )
	{
		if( m_mouses[ i ]->GetNextEvtTimestamp() == timestamp )
			return DispatchedEvent( m_mouses[ i ].get(), m_mouses[ i ]->ApplyNextEvent() );
	}

	for( int i = 0; i < m_keyboards.size(); i++ )
	{
		if( m_keyboards[ i ]->GetNextEvtTimestamp() == timestamp )
			return DispatchedEvent( m_keyboards[ i ].get(), m_keyboards[ i ]->ApplyNextEvent() );
	}

	for( int i = 0; i < m_joysticks.size(); i++ )
	{
		if( m_joysticks[ i ]->GetNextEvtTimestamp() == timestamp )
			return DispatchedEvent( m_joysticks[ i ].get(), m_joysticks[ i ]->ApplyNextEvent() );
	}

	return InputDispatcher::DispatchedEvent( nullptr, DeviceEvent() );
}

// ================================ //
//
inline Timestamp							InputDispatcher::FindMinTimestamp()
{
	Timestamp minTimestamp = std::numeric_limits< Timestamp >::max();

	// Mouse events are will be the most often.
	for( int i = 0; i < m_mouses.size(); i++ )
	{
		if( m_mouses[ i ]->GetNextEvtTimestamp() < minTimestamp )
			minTimestamp = m_mouses[ i ]->GetNextEvtTimestamp();
	}

	for( int i = 0; i < m_keyboards.size(); i++ )
	{
		if( m_keyboards[ i ]->GetNextEvtTimestamp() < minTimestamp )
			minTimestamp = m_keyboards[ i ]->GetNextEvtTimestamp();
	}

	for( int i = 0; i < m_joysticks.size(); i++ )
	{
		if( m_joysticks[ i ]->GetNextEvtTimestamp() < minTimestamp )
			minTimestamp = m_joysticks[ i ]->GetNextEvtTimestamp();
	}

	return minTimestamp;
}

}	// input
}	// sw

