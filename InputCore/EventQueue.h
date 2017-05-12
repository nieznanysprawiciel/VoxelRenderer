#pragma once
/**
@file EventQueue.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/TypesDefinitions.h"
#include "InputDeviceEvent.h"

#include <vector>



namespace sw {
namespace input
{


/**@brief Abstraction of event queue.

This is not normal queue. You can always access events that have been already read and not cleared.
Events are ersaed only by explicit call to ClearReadEvents.*/
template< typename EventType >
class EventQueue
{
private:

	std::vector< EventType >			m_events;
	uint32								m_readPointer;

public:

	explicit		EventQueue		();
					~EventQueue		() = default;

	/**@brief Adds event on the end of queue.*/
	inline void		AddEvent		( const EventType& event );

	/**@brief Removes all events after m_readPointer.*/
	inline void		ClearReadEvents	();
	inline bool		NoMoreEvents	() const						{ return (uint32)m_events.size() <= m_readPointer; }
	inline uint32	EventsLeft		() const						{ return (uint32)m_events.size() - m_readPointer; }

	/**@brief Reads next event without moving read pointer.
	Always check if there're any events in queue left.*/
	inline const EventType&				FrontEvent		() const	{ assert( !NoMoreEvents() ); return m_events[ m_readPointer ]; }

	/**@brief Gets next event and moves m_readPointer.*/
	inline const EventType&				PopEvent		();

	/**@brief Gets whole array of events.
	This array consists of events to */
	const std::vector< EventType >&		GetEvents		() const	{ return m_events; }

	/**@brief Gets index of next event to read.*/
	uint32								GetReadPointer	() const	{ return m_readPointer; }
};


//====================================================================================//
//			Implementation	
//====================================================================================//

template< typename EventType >
inline EventQueue< EventType >::EventQueue()
	:	m_readPointer( 0 )
{}


// ================================ //
//
template< typename EventType >
inline void					EventQueue< EventType >::AddEvent			( const EventType& event )
{
	m_events.push_back( event );
}

// ================================ //
//
template< typename EventType >
inline void					EventQueue< EventType >::ClearReadEvents	()
{
	m_events.erase( m_events.begin(), m_events.begin() + m_readPointer );
	m_readPointer = 0;
}

// ================================ //
//
template< typename EventType >
inline const EventType&		EventQueue< EventType >::PopEvent			()
{
	assert( !NoMoreEvents() );
	return m_events[ m_readPointer++ ];
}

}	// input
}	// sw


