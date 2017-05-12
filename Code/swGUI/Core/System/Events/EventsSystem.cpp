/**
@file EventsSystem.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EventsSystem.h"
#include "EventsSystem.inl"

#include "swGUI/Core/Controls/UIElement.h"



namespace sw {
namespace gui
{


// ================================ //
// Allows to insert RegisteredEvent to map.
bool				operator<		( const RegisteredEvent& left, const RegisteredEvent& right )
{
	if( left.OwnerType < right.OwnerType )
		return true;
	else if( left.OwnerType == right.OwnerType )
	{
		if( left.EventArgumentsType < right.EventArgumentsType )
			return true;
		else if( left.EventArgumentsType == right.EventArgumentsType )
		{
			if( left.EventName < right.EventName )
				return true;
		}
	}

	return false;
}


// ================================ //
//
EventsSystem::EventsSystem()
	:	m_counter( 1 )
{}


// ================================ //
//
const RegisteredEvent*			EventsSystem::RegisterEvent		( const char* eventName, RoutingStrategy strategy, TypeID ownerType, TypeID eventTypeId )
{
	RegisteredEvent eventInfo( eventName, strategy, ownerType, eventTypeId );

	std::lock_guard< std::mutex > lock( m_registerMutex );
	
	eventInfo.ID = m_counter;

	// Note: if new entry have been added, we must increment m_counter. Otherwise event was already registered
	// and we use object stored in m_registeredEvents set.
	auto iter = m_registeredEvents.insert( eventInfo );
	if( iter.second )
		m_counter++;

	return &(*iter.first);
}

// ================================ //
//
bool				EventsSystem::RaiseEvent					( const RegisteredEvent* eventInfo, UIElement* sender, IEventArgsOPtr&& arguments )
{
	return RaiseEventImpl< IEventArgs >( eventInfo, sender, arguments.get(), nullptr );
}

// ================================ //
//
bool				EventsSystem::RaiseForwardEvent				( const RegisteredEvent* eventInfo, UIElement* sender, IEventArgs* arguments )
{
	return RaiseEventImpl< IEventArgs >( eventInfo, sender, arguments, nullptr );
}

// ================================ //
//
EventsSystem&		EventsSystem::Get()
{
	static EventsSystem manager;
	return manager;
}





}	// gui
}	// sw