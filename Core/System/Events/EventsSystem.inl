#pragma once
/**
@file EventsSystem.inl
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EventsSystem.h"


namespace sw {
namespace gui
{



// ================================ //
//
template< typename ArgumentType >
inline bool					EventsSystem::RaiseEvent			( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, EventHandlerPointer< ArgumentType > handler )
{
	return RaiseEventImpl< ArgumentType >( eventInfo, sender, arguments, handler );
}

// ================================ //
//
template< typename ArgumentType >
inline bool					EventsSystem::RaiseDirectEvent		( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, EventHandlerPointer< ArgumentType > handler )
{
	if( handler )
		( sender->*handler )( sender, arguments );

	sender->InvokeEventDelegates( eventInfo->ID, sender, arguments, AccessKey() );
	return true;
}

// ================================ //
//
template< typename ArgumentType >
inline bool					EventsSystem::RaiseBubbleEvent		( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, EventHandlerPointer< ArgumentType > handler )
{
	UIElement* element = sender;
	while( element && !arguments->Handled )
	{
		if( handler )
			( element->*handler )( sender, arguments );

		element->InvokeEventDelegates( eventInfo->ID, sender, arguments, AccessKey() );
		element = element->GetParent();
	}

	return true;
}

// ================================ //
//
template< typename ArgumentType >
inline bool					EventsSystem::RaiseTunnelEvent		( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, EventHandlerPointer< ArgumentType > handler )
{
	RaiseTunnelEventForParents< ArgumentType >( eventInfo, sender, arguments, sender, handler );
	return true;
}

// ================================ //
//
template< typename ArgumentType >
inline void					EventsSystem::RaiseTunnelEventForParents	( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, UIElement* raiseForElement, EventHandlerPointer< ArgumentType > handler )
{
	if( !raiseForElement )
		return;

	RaiseTunnelEventForParents< ArgumentType >( eventInfo, sender, arguments, raiseForElement->GetParent(), handler );

	if( handler )
		( raiseForElement->*handler )( sender, arguments );

	if( !arguments->Handled )
		raiseForElement->InvokeEventDelegates( eventInfo->ID, sender, arguments, AccessKey() );
}


// ================================ //
//
template< typename ArgumentType >
inline bool					EventsSystem::RaiseEventImpl				( const RegisteredEvent* eventInfo, UIElement* sender, ArgumentType* arguments, EventHandlerPointer< ArgumentType > handler )
{
	// Type checking. If sender is different then registered then something gone wrong.
	// Argument checking could be done only in debug mode...
	if( !sender->GetType().is_derived_from( eventInfo->OwnerType ) )
		return false;

	if( !arguments->get_type().is_derived_from( eventInfo->EventArgumentsType ) )
		return false;

	switch( eventInfo->Strategy )
	{
		case RoutingStrategy::Direct:
			return RaiseDirectEvent< ArgumentType >( eventInfo, sender, arguments, handler );
		case RoutingStrategy::Bubble:
			return RaiseBubbleEvent< ArgumentType >( eventInfo, sender, arguments, handler );
		case RoutingStrategy::Tunnel:
			return RaiseTunnelEvent< ArgumentType >( eventInfo, sender, arguments, handler );
		default:
			return false;
	}
}


}	// gui
}	// sw

