#include "TestUIElementClass.h"
#include "swCommonLib/External/Catch/catch.hpp"


// ================================ //
//
RTTR_REGISTRATION
{
	rttr::registration::class_< TestUIElementClass >( "TestUIElementClass" );
	rttr::registration::class_< ValidationEventArgs >( "ValidationEventArgs" );
	rttr::registration::class_< UnusedEventArgs >( "UnusedEventArgs" );
}


using namespace sw::gui;




//====================================================================================//
//			Definitions	
//====================================================================================//


REGISTER_EVENT_DEFINITION( ValidationStarted, RoutingStrategy::Direct, TestUIElementClass, ValidationEventArgs );
REGISTER_EVENT_DEFINITION( ValidationEnded, RoutingStrategy::Direct, TestUIElementClass, ValidationEventArgs );
REGISTER_EVENT_DEFINITION( UnusedEvent, RoutingStrategy::Direct, TestUIElementClass, UnusedEventArgs );

REGISTER_EVENT_DEFINITION( PreviewTunnelEvent, RoutingStrategy::Tunnel, TestUIElementClass, ValidationEventArgs );
REGISTER_EVENT_DEFINITION( BubbleEvent, RoutingStrategy::Bubble, TestUIElementClass, ValidationEventArgs );


// ================================ //
//
void		TestUIElementClass::EventRaisingFunction		()
{
	bool result = ValidationStarted().RaiseEvent( this, ValidationEventArgsOPtr( new ValidationEventArgs() ) );
	CHECK( result );

	result = UnusedEvent().RaiseEvent( this, UnusedEventArgsOPtr( new UnusedEventArgs() ) );
	CHECK( result );
}

// ================================ //
//
void			TestUIElementClass::RaiseTunnelEvent()
{
	bool result = PreviewTunnelEvent().RaiseEvent( this, ValidationEventArgsOPtr( new ValidationEventArgs() ) );
	CHECK( result );
}

// ================================ //
//
void			TestUIElementClass::RaiseBubbleEvent()
{
	bool result = BubbleEvent().RaiseEvent( this, ValidationEventArgsOPtr( new ValidationEventArgs() ) );
	CHECK( result );
}

// ================================ //
//
bool			TestUIElementClass::ExistsDelegatesContainer	( ::sw::gui::EventType eventID )
{
	return m_eventHandlers.FindContainer( eventID ) ? true : false;
}

// ================================ //
//
Size			TestUIElementClass::GetNumChildren() const
{
	return m_children.size();
}

// ================================ //
//
UIElement *		TestUIElementClass::GetUIChild( Size idx ) const
{
	if( idx < m_children.size() )
		return m_children[ idx ].get();
	return nullptr;
}

// ================================ //
//
bool			TestUIElementClass::AddChild( sw::gui::UIElementOPtr && child )
{
	if( !child )
		return false;

	UIElement* childPtr = child.get();

	m_children.push_back( std::move( child ) );
	childPtr->SetParent( this );

	return true;
}


