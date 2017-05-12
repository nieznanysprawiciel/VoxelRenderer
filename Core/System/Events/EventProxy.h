#pragma once
/**
@file EventProxy.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "DelegateContainer.h"
#include "EventHandlers.h"
#include "RegisteredEvent.h"


namespace sw {
namespace gui
{


/**@brief Helper class for accessing event.

@ingroup EventSystemImpl*/
template< typename EventArgType >
class EventProxy
{
public:
	typedef EventDelegate< EventArgType >		DelegateType;
	typedef std::unique_ptr< EventArgType >		ArgumentsOwnerPtr;
private:

	EventHandlers&							m_handlers;
	const RegisteredEvent* 					m_eventInfo;

public:
	explicit			EventProxy		( EventHandlers& delegatesContainer, const RegisteredEvent* eventInfo );


	inline void			operator+=		( DelegateType delegate );
	inline bool			operator-=		( DelegateType delegate );
	inline void			AddDelegate		( DelegateType delegate );
	inline bool			RemoveDelegate	( DelegateType delegate );

	bool				RaiseEvent		( UIElement* sender, ArgumentsOwnerPtr&& arguments );
};


//====================================================================================//
//			Implementation	
//====================================================================================//

// ================================ //
//
template< typename EventArgType >
inline					EventProxy< EventArgType >::EventProxy		( EventHandlers& delegatesContainer, const RegisteredEvent* eventInfo )
	:	m_handlers( delegatesContainer )
	,	m_eventInfo( eventInfo )
{}

// ================================ //
//
template< typename EventArgType >
inline void				EventProxy< EventArgType >::operator+=		( typename EventProxy< EventArgType >::DelegateType delegate )
{
	AddDelegate( delegate );
}

// ================================ //
//
template< typename EventArgType >
inline bool				EventProxy< EventArgType >::operator-=		( typename EventProxy< EventArgType >::DelegateType delegate )
{
	return RemoveDelegate( delegate );
}

// ================================ //
//
template< typename EventArgType >
inline void				EventProxy< EventArgType >::AddDelegate		( typename EventProxy< EventArgType >::DelegateType delegate )
{
	// Find delegates container.
	DelegatesContainerBase* delegateContainer = m_handlers.FindContainer( m_eventInfo->ID );
	DelegatesContainer< EventArgType >* typedDelegateContainer = nullptr;

	// Create delegates container if it doesn't exists.
	if( !delegateContainer )
	{
		DelegatesContainer< EventArgType >* container = new DelegatesContainer< EventArgType >( m_eventInfo->ID );
		delegateContainer = m_handlers.AddContainer( std::move( DelegatesContainerBaseOPtr( container ) ) );
	}

	typedDelegateContainer = static_cast< DelegatesContainer< EventArgType >* >( delegateContainer );
	typedDelegateContainer->AddDelegate( delegate );
}

// ================================ //
//
template< typename EventArgType >
inline bool				EventProxy< EventArgType >::RemoveDelegate	( typename EventProxy< EventArgType >::DelegateType delegate )
{
	DelegatesContainerBase* delegateContainer = m_handlers.FindContainer( m_eventInfo->ID );

	if( delegateContainer )
	{
		DelegatesContainer< EventArgType >* typedDelegateContainer = static_cast< DelegatesContainer< EventArgType >* >( delegateContainer );
		bool result = typedDelegateContainer->RemoveDelegate( delegate );
		
		// Remove container if isn't usufull anymore.
		if( typedDelegateContainer->IsEmpty() )
		{
			m_handlers.RemoveContainer( m_eventInfo->ID );
		}

		return result;
	}
	return false;
}

// ================================ //
//
template< typename EventArgType >
inline bool				EventProxy< EventArgType >::RaiseEvent		( UIElement* sender, ArgumentsOwnerPtr&& arguments )
{
	return m_handlers.RaiseEvent( m_eventInfo, sender, std::move( arguments ) );
}

}	// gui
}	// sw