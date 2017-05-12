#pragma once
/**
@file EventHandlers.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "DelegateContainer.h"
#include "IEventArgs.h"


namespace sw {
namespace gui
{

struct IEventArgs;
struct RegisteredEvent;


/**@brief Container class for delegates.

@ingroup EventSystem*/
class EventHandlers
{
	template< typename EventArgType > friend class EventProxy;
private:

	DelegatesContainerBaseOPtr		m_delegatesList;

public:
	explicit			EventHandlers();

	/**@brief Search for events container.
	@return This function can return nullptr if container haven't been created yet.*/
	DelegatesContainerBase*		FindContainer		( EventType eventID );

private:
	///@name Functions for EventProxy
	///@{

	/**@brief Adds delegates container.
	@return If container already existed, this function returns this container.*/
	DelegatesContainerBase*		AddContainer		( DelegatesContainerBaseOPtr&& container );

	/**@brief Removes container with deelgates.
	@return Returns false if container for eventID didn't exist.*/
	bool						RemoveContainer		( EventType eventID );

	/**@brief Sends event to visual tree using @ref RoutingStrategy specyfied in eventInfo.
	Calls @ref EventsSystem::RaiseEvent.
	@return Returns false if event couldn't be sent. False means you provided wrong argument type for this event,
	or sender type is different then registered. Normally you don't have to check this.*/
	bool						RaiseEvent			( const RegisteredEvent* eventInfo, UIElement* sender, IEventArgsOPtr&& arguments );

	///@}
	
private:
	// Internal

	/**@brief Checks if container exists.*/
	bool						Exists				( EventType eventID );
};




}	// gui
}	// sw

#define REGISTER_EVENT_DECLARATION( name, strategy, className, argsType )				\
static const ::sw::gui::RegisteredEvent* s ## name;										\
::sw::gui::EventProxy< argsType >		name() { return ::sw::gui::EventProxy< argsType >( m_eventHandlers, s ## name ); }

#define REGISTER_EVENT_DEFINITION( name, strategy, className, argsType )				\
const ::sw::gui::RegisteredEvent* className::s ## name = ::sw::gui::EventsSystem::Get().RegisterEvent( #name, ::sw::gui::strategy, TypeID::get< className >(), TypeID::get< argsType >() );

