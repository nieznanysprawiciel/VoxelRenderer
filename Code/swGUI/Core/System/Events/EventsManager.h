#pragma once
/**
@file EventsManager.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/Common/RTTR.h"


#include <mutex>
#include <set>


namespace sw {
namespace gui
{


typedef uint32 EventType;



/**@brief */
struct RegisteredEvent
{
	const char*			EventName;
	TypeID				OwnerType;
	TypeID				EventArgumentsType;
	EventType			ID;

// ================================ //
//
	RegisteredEvent( const char* eventName, TypeID ownerType, TypeID eventTypeId )
		: EventName( eventName )
		, OwnerType( ownerType )
		, EventArgumentsType( eventTypeId )
	{}
};

bool		operator<( const RegisteredEvent& left, const RegisteredEvent& right );


/**@brief Events management.

For now this class stores only registered events. Think about future use.

Registering events needs to be in separate dll, otherwise controls can't register their static variables.*/
class EventsManager
{
private:

	EventType						m_counter;

	std::mutex						m_registerMutex;
	std::set< RegisteredEvent >		m_registeredEvents;

protected:
public:
					~EventsManager		() = default;

	/**@brief Registers new event and return idetifier used in DelegateContainers.*/
	EventType		RegisterEvent		( const char* eventName, TypeID ownerType, TypeID eventTypeId );



public:
	static EventsManager&		Get	();

private:
	explicit		EventsManager		();
};


}	// gui
}	// sw
