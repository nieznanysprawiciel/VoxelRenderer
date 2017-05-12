#pragma once
/**
@file DelegateContainer.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/External/FastDelegate/FastDelegate.h"

#include "RegisteredEvent.h"

#include <vector>




namespace sw {
namespace gui
{



class DelegatesContainerBase;
struct IEventArgs;
class UIElement;
DEFINE_OPTR_TYPE( DelegatesContainerBase );

template< typename EventArgType >
using EventDelegate = fastdelegate::FastDelegate2< UIElement*, EventArgType* >;


/**@brief Base class for delegates containers.

Purpose of this class is to provide interface for storing delegates of different types in one list.
Derived classes are templates that can hold typed delegates. Note that each derived class can correspond
to multiple events, because different event can be sent with the same structure that holds parameters.

Thats why there's m_eventType field. While looking for appropriate container EventManager will check this
field to compare it with sent event type.

This class is designed to minimize memory footprint in Control classes. That's why I use intrusive list to store
consecutive containers. Since there're only few delegates used at the same time, there's no sense to expose
DelegatesContainer for each event.

This class owns next delegates container and is responsible for releasing memory.

@todo Consider extracting intrusive list as separate class.

@ingroup EventSystemImpl*/
class DelegatesContainerBase
{
private:

	DelegatesContainerBaseOPtr	m_next;			///< Intrusive list.
	EventType					m_eventType;	///< Type of event used to distinguish between Delegates Containers.

protected:
public:
	explicit		DelegatesContainerBase		( EventType eventID );
	virtual			~DelegatesContainerBase		() = default;


	/**@brief Call this function to check event type holded by derived class.*/
	inline EventType			GetEventType		()		{ return m_eventType; }


	/**@brief Gets next element on linked list.*/
	DelegatesContainerBase*		GetNextContainer	()		{ return m_next.get(); }
	
	/**@brief Adds container on the end of list.*/
	void						AddContainer		( DelegatesContainerBaseOPtr&& container );
	
	/**@brief Removed element following this object.*/
	void						RemoveNext			();

	/**@brief Steal list following this */
	DelegatesContainerBaseOPtr	StealRestOfList		();

	/**@brief Invokes delegates functions.*/
	virtual void				CallDelegates		( UIElement* sender, IEventArgs* arguments ) = 0;
};



/**@brief Derived template class for delegates.

Check description in @ref DelegatesContainerBase.*/
template< typename EventArgType >
class DelegatesContainer : public DelegatesContainerBase
{
public:

	typedef EventDelegate< EventArgType > DelegateType;

private:

	std::vector< DelegateType >		m_delegates;		///< Delegates for EventType defined in @ref DelegatesContainerBase. @todo Consider using linked list to minimize memory usage.

protected:
public:
	explicit		DelegatesContainer		( EventType type );
	virtual			~DelegatesContainer		() = default;


	inline void			operator+=		( DelegateType delegate );
	void				AddDelegate		( DelegateType delegate );
	bool				RemoveDelegate	( DelegateType delegate );

	bool				Exists			( DelegateType delegate );
	bool				IsEmpty			() const;

	
	virtual void		CallDelegates	( UIElement* sender, IEventArgs* arguments ) override;
};


//====================================================================================//
//			Implementation	
//====================================================================================//

// ================================ //
//
inline DelegatesContainerBase::DelegatesContainerBase	( EventType eventID )
	:	m_eventType( eventID )
	,	m_next( nullptr )
{}

// ================================ //
//
inline void				DelegatesContainerBase::AddContainer	( DelegatesContainerBaseOPtr&& container )
{
	if( !m_next )
		m_next = std::move( container );
	else
		m_next->AddContainer( std::move( container ) );
}

// ================================ //
//
inline void				DelegatesContainerBase::RemoveNext		()
{
	if( !m_next )
		return;

	// This will release our m_next and steal followers m_next;
	m_next = std::move( m_next->m_next );
}

// ================================ //
//
inline DelegatesContainerBaseOPtr	DelegatesContainerBase::StealRestOfList()
{
	return std::move( m_next );
}



//====================================================================================//
//				DelegatesContainer
//====================================================================================//


// ================================ //
//
template< typename EventArgType >
inline					DelegatesContainer< EventArgType >::DelegatesContainer	( EventType type )
	:	DelegatesContainerBase( type )
{}

// ================================ //
//
template< typename EventArgType >
inline void				DelegatesContainer< EventArgType >::operator+=		( typename DelegatesContainer< EventArgType >::DelegateType delegate )
{
	AddDelegate( delegate );
}

// ================================ //
//
template< typename EventArgType >
inline void				DelegatesContainer< EventArgType >::AddDelegate		( typename DelegatesContainer< EventArgType >::DelegateType delegate )
{
	if( !Exists( delegate ) )
		m_delegates.push_back( delegate );
}

// ================================ //
//
template< typename EventArgType >
inline bool				DelegatesContainer< EventArgType >::RemoveDelegate	( typename DelegatesContainer< EventArgType >::DelegateType delegate )
{
	bool removed = false;
	for( auto iter = m_delegates.begin(); iter != m_delegates.end(); )
	{
		if( *iter == delegate )
		{
			iter = m_delegates.erase( iter );
			removed = true;
		}
		else
			iter++;
	}

	return removed;
}

// ================================ //
//
template< typename EventArgType >
inline bool				DelegatesContainer< EventArgType >::Exists			( typename DelegatesContainer< EventArgType >::DelegateType delegate )
{
	for( auto& exisitingDelegate : m_delegates )
	{
		if( exisitingDelegate == delegate )
			return true;
	}
	return false;
}

// ================================ //
//
template< typename EventArgType >
inline bool				DelegatesContainer< EventArgType >::IsEmpty() const
{
	return m_delegates.empty();
}

// ================================ //
//
template< typename EventArgType >
inline void				DelegatesContainer< EventArgType >::CallDelegates	( UIElement* sender, IEventArgs* arguments )
{
	// @todo What should be done if event will be marked as handled ??
	EventArgType* typedArgs = static_cast< EventArgType* >( arguments );
	for( auto& delegateFun : m_delegates )
		delegateFun( sender, typedArgs );
}

}	// gui
}	// sw


