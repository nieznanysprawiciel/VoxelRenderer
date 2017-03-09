/**
@file EventsManager.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "EventsManager.h"


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
			auto result = strcmp( left.EventName, right.EventName );
			if( result < 0 )
				return true;
		}
	}

	return false;
}


// ================================ //
//
EventsManager::EventsManager()
	:	m_counter( 1 )
{}


// ================================ //
//
EventType			EventsManager::RegisterEvent		( const char* eventName, TypeID ownerType, TypeID eventTypeId )
{
	RegisteredEvent eventInfo( eventName, ownerType, eventTypeId );

	std::lock_guard< std::mutex > lock( m_registerMutex );
	
	auto iter = m_registeredEvents.insert( eventInfo );
	if( iter.second )
	{
		// Sad
		const_cast< RegisteredEvent& >( *(iter.first) ).ID = m_counter++;
	}

	return iter.first->ID;
}



// ================================ //
//
EventsManager&		EventsManager::Get()
{
	static EventsManager manager;
	return manager;
}





}	// gui
}	// sw