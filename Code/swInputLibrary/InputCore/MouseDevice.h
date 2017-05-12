#pragma once
/**
@file MouseDevice.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "Device.h"

#include "MouseState.h"
#include "InputDeviceInfo.h"
#include "InputDeviceEvent.h"

#include "EventQueue.h"

#include <vector>


namespace sw {
namespace input
{


/**@brief Mouse state and events.

@ingroup Input*/
class MouseDevice : public Device
{
private:

	InputDeviceInfo		m_info;
	MouseState			m_state;

	EventQueue< DeviceEvent >		m_events;

protected:
public:
	explicit		MouseDevice		() = default;
					~MouseDevice	() = default;


	const InputDeviceInfo&		GetInfo			() const	{ return m_info; }
	const MouseState&			GetState		() const	{ return m_state;  }

	EventQueue< DeviceEvent >&	GetEventsQueue	()			{ return m_events; }

	void						ApplyAllEvents			();
	DeviceEvent					ApplyNextEvent			();
	Timestamp					GetNextEvtTimestamp		();

	void						RemoveEvents			();

public:
	///@name Functions for setting state (only for IInput)
	///@{

	/**@brief Add event to event queue.
	This function doesn't change KeyboardState.*/
	void			AddEvent		( const DeviceEvent& event );

	///@}
};


DEFINE_OPTR_TYPE( MouseDevice );

//====================================================================================//
//			Inline implementation	
//====================================================================================//

// ================================ //
//
inline void			MouseDevice::ApplyAllEvents	()
{
	while( !m_events.NoMoreEvents() )
	{
		auto& event = m_events.PopEvent();
		m_state.ApplyEvent( event );
	}
}

// ================================ //
//
inline DeviceEvent	MouseDevice::ApplyNextEvent()
{
	if( m_events.NoMoreEvents() )
		return DeviceEvent();

	auto& nextEvent = m_events.PopEvent();
	m_state.ApplyEvent( nextEvent );

	return nextEvent;
}

// ================================ //
//
inline Timestamp	MouseDevice::GetNextEvtTimestamp()
{
	if( m_events.NoMoreEvents() )
		return std::numeric_limits< Timestamp >::max();

	return m_events.FrontEvent().LogicalTime;
}

// ================================ //
//
inline void			MouseDevice::RemoveEvents		()
{
	m_state.RemoveEvents();
	m_events.ClearReadEvents();
}


// ================================ //
//
inline void			MouseDevice::AddEvent		( const DeviceEvent& event )
{
	m_events.AddEvent( event );
}




}	// input
}	// sw


