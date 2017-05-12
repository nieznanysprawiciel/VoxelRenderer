#pragma once
/**
@file EventsSystem.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/Common/RTTR.h"

#include "EventProxy.h"
#include "EventHandlers.h"
#include "IEventArgs.h"
#include "RegisteredEvent.h"


#include <mutex>
#include <set>
#include <string>


/**@defgroup EventSystem Events System
@brief Sending and receiving events.

@ingroup GUICore*/

/**@defgroup EventSystemImpl Events System Implementation
@brief Detailed implementation of events system. 
Here are things that shouldn't interest ordinary users.
@ingroup EventSystem*/


namespace sw {
namespace gui
{

class UIElement;


template< typename ArgumentType >
using EventHandlerPointer = void ( UIElement::* )( UIElement*, ArgumentType* );


/**@brief Events management.

For now this class stores only registered events. Think about future use.

Registering events needs to be in separate dll, otherwise controls can't register their static variables.

All calsses derived from @ref UIElement can register they own events.

@ingroup EventSystem*/
class EventsSystem
{
private:

	EventType						m_counter;

	std::mutex						m_registerMutex;
	std::set< RegisteredEvent >		m_registeredEvents;

protected:
public:
					~EventsSystem		() = default;

	/**@brief Registers new event and return idetifier used in DelegateContainers.*/
	const RegisteredEvent*		RegisterEvent		( const char* eventName, RoutingStrategy strategy, TypeID ownerType, TypeID eventTypeId );

	/**@brief Sends event to visual tree using @ref RoutingStrategy specyfied in eventInfo.*/
	bool						RaiseEvent			( const RegisteredEvent* eventInfo, UIElement* sender, IEventArgsOPtr&& arguments );

	/**@brief Sends event in response to other event.
	Use this function when you are in event handler and you want to send the same event structure under different name.
	Good example is UIElement::MouseRightButtonDown which is sent by UIElement in response to UIElement::MouseDown event
	sent by EventsSystem.

	@param[in] arguments Arguments structure. Note that this structure is raw pointer and you are not owner of it.
	Never use this function when you created event structure on your own. This couses memory leaks.*/
	bool						RaiseForwardEvent	( const RegisteredEvent* eventInfo, UIElement* sender, IEventArgs* arguments );

	template< typename ArgumentType >
	bool						RaiseEvent			( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, EventHandlerPointer< ArgumentType > handler );


private:

	template< typename ArgumentType >
	bool			RaiseDirectEvent			( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, EventHandlerPointer< ArgumentType > handler );

	template< typename ArgumentType >
	bool			RaiseBubbleEvent			( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, EventHandlerPointer< ArgumentType > handler );

	template< typename ArgumentType >
	bool			RaiseTunnelEvent			( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, EventHandlerPointer< ArgumentType > handler );

	/**@brief Calls recursivly event for parents.*/
	template< typename ArgumentType >
	void			RaiseTunnelEventForParents	( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, UIElement* raiseForElement, EventHandlerPointer< ArgumentType > handler );

	template< typename ArgumentType >
	bool			RaiseEventImpl				( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, EventHandlerPointer< ArgumentType > handler );

public:
	static EventsSystem&		Get	();

private:
	explicit		EventsSystem		();
};





}	// gui
}	// sw
