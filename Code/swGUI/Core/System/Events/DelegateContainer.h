#pragma once
/**
@file DelegateContainer.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "EventsManager.h"
#include "swCommonLib/External/FastDelegate/FastDelegate.h"


#include <vector>




namespace sw {
namespace gui
{

class DelegatesContainerBase;
DEFINE_OPTR_TYPE( DelegatesContainerBase );


/**@brief Base class for delegates containers.

Purpose of this class is to provide interface for storing delegates of different types in one list.
Derived classes are templates that can hold typed delegates. Note that each derived class can correspond
to multiple events, because different event can be sent with the same structure that holds parameters.

Thats why there's m_eventType field. While looking for appropriate container EventManager will check this
field to compare it with sent event type.

This class is designed to minimize memory footprint in Control classes. That's why I use intrusive list to store
consecutive containers. Since there're only few delegates used at the same time, there's no sense to expose
DelegatesContainer for each event.

This class owns next delegates container and is responsible for releasing memory.*/
class DelegatesContainerBase
{
private:

	DelegatesContainerBaseOPtr	m_next;			///< Intrusive list.
	EventType					m_eventType;	///< Type of event used to distinguish between Delegates Containers.

protected:
public:
	explicit		DelegatesContainerBase		( EventType type );
	virtual			~DelegatesContainerBase		() = default;


	/**@brief Call this function to check event type holded by derived class.*/
	inline EventType			GetEventType		()		{ return m_eventType; }


	/**@brief Gets next element on linked list.*/
	DelegatesContainerBase*		GetNextContainer	()		{ return m_next.get(); }
	/**@brief Adds container on the end of list.*/
	void						AddContainer		( DelegatesContainerBaseOPtr&& container );
	/**@brief Removed element following this object.*/
	void						RemoveNext			();
};



/**@brief Derived template class for delegates.

Check description in @ref DelegatesContainerBase.*/
template< typename EventArgType >
class DelegatesContainer : public DelegatesContainerBase
{
public:

	typedef fastdelegate::FastDelegate1< EventArgType > DelegateType;

private:

	std::vector< DelegateType >		m_delegates;		///< Delegates for EventType defined in @ref DelegatesContainerBase. @todo Consider using linked list to minimize memory usage.

protected:
public:
	explicit		DelegatesContainer		( EventType type );
	virtual			~DelegatesContainer		() = default;


	inline void			operator+=		( DelegateType delgate );
	void				AddDelegate		( DelegateType delgate );
	bool				RemoveDelegate	( DelegateType delgate );

	bool				Exists			( DelegateType delgate );
};


//====================================================================================//
//			Implementation	
//====================================================================================//

// ================================ //
//
DelegatesContainerBase::DelegatesContainerBase	( EventType type )
	:	m_eventType( type )
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



//====================================================================================//
//				DelegatesContainer
//====================================================================================//


// ================================ //
//
template< typename DelegateType >
inline					DelegatesContainer< DelegateType >::DelegatesContainer	( EventType type )
	:	DelegatesContainerBase( type )
{}

// ================================ //
//
template< typename DelegateType >
inline void				DelegatesContainer< DelegateType >::operator+=		( DelegateType delgate )
{
	AddDelegate( delegate );
}

// ================================ //
//
template< typename DelegateType >
inline void				DelegatesContainer< DelegateType >::AddDelegate		( DelegateType delgate )
{
	if( !Exists( delegate ) )
		m_delegates.push_back( delegate );
}

// ================================ //
//
template< typename DelegateType >
inline bool				DelegatesContainer< DelegateType >::RemoveDelegate	( DelegateType delgate )
{
	for( auto iter = m_delegates.begin(); iter != m_delegates.end(); iter++ )
	{
		if( *iter == delegate )
			iter = m_delegates.erase( iter );
	}
}

// ================================ //
//
template< typename DelegateType >
inline bool				DelegatesContainer< DelegateType >::Exists			( DelegateType delgate )
{
	for( auto& exisitingDelegate : m_delegates )
	{
		if( exisitingDelegate == delegate )
			return true;
	}
	return false;
}

}	// gui
}	// sw


