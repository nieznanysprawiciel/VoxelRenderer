/**
@file EventHandlers.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EventHandlers.h"
#include "IEventArgs.h"

#include "EventsSystem.h"


namespace sw {
namespace gui
{


// ================================ //
//
EventHandlers::EventHandlers()
	:	m_delegatesList( nullptr )
{}

// ================================ //
//
DelegatesContainerBase*			EventHandlers::FindContainer		( EventType eventID )
{
	DelegatesContainerBase* container = m_delegatesList.get();
	while( container )
	{
		if( container->GetEventType() == eventID )
			return container;

		container = container->GetNextContainer();
	}

	return nullptr;
}


//====================================================================================//
//			Functions for EventProxy	
//====================================================================================//

// ================================ //
//
DelegatesContainerBase*		EventHandlers::AddContainer				( DelegatesContainerBaseOPtr&& container )
{
	EventType eventID = container->GetEventType();
	auto existingContainer = FindContainer( eventID );

	if( existingContainer )
		return existingContainer;

	if( m_delegatesList )
		m_delegatesList->AddContainer( std::move( container ) );
	else
		m_delegatesList = std::move( container );

	return FindContainer( eventID );
}

// ================================ //
//
bool						EventHandlers::RemoveContainer			( EventType eventID )
{
	if( !m_delegatesList )
		return false;

	DelegatesContainerBase* prevContainer = m_delegatesList.get();
	DelegatesContainerBase* container = nullptr;

	if( prevContainer->GetEventType() == eventID )
	{
		m_delegatesList = prevContainer->StealRestOfList();
		return true;
	}
	else
	{
		container = prevContainer->GetNextContainer();
		while( container )
		{
			if( container->GetEventType() == eventID )
			{
				prevContainer->RemoveNext();
				return true;
			}

			prevContainer = container;
			container = container->GetNextContainer();
		}
	}

	return false;
}

// ================================ //
//
bool						EventHandlers::RaiseEvent				( const RegisteredEvent* eventInfo, UIElement* sender, IEventArgsOPtr&& arguments )
{
	return EventsSystem::Get().RaiseEvent( eventInfo, sender, std::move( arguments ) );
}


//====================================================================================//
//			Internal	
//====================================================================================//

// ================================ //
//
bool						EventHandlers::Exists					( EventType eventID )
{
	return FindContainer( eventID ) ? true : false;
}




}	// gui
}	// sw