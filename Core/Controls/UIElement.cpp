/**
@file UIElement.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "UIElement.h"
#include "swGUI/Core/System/HostWindow.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< sw::gui::DependencyObject >( "sw::gui::DependencyObject" );
	rttr::registration::class_< sw::gui::Visual >( "sw::gui::Visual" );
	rttr::registration::class_< sw::gui::UIElement >( "sw::gui::UIElement" );
}



namespace sw {
namespace gui
{

//====================================================================================//
//			Events definition	
//====================================================================================//

REGISTER_EVENT_DEFINITION( PreviewMouseUp, RoutingStrategy::Tunnel, UIElement, MouseButtonEventArgs );
REGISTER_EVENT_DEFINITION( PreviewMouseDown, RoutingStrategy::Tunnel, UIElement, MouseButtonEventArgs );

REGISTER_EVENT_DEFINITION( PreviewMouseRightButtonUp, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
REGISTER_EVENT_DEFINITION( PreviewMouseRightButtonDown, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
REGISTER_EVENT_DEFINITION( PreviewMouseLeftButtonDown, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
REGISTER_EVENT_DEFINITION( PreviewMouseLeftButtonUp, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );

REGISTER_EVENT_DEFINITION( PreviewMouseWheel, RoutingStrategy::Tunnel, UIElement, MouseWheelEventArgs );
REGISTER_EVENT_DEFINITION( PreviewMouseMove, RoutingStrategy::Tunnel, UIElement, MouseEventArgs );

REGISTER_EVENT_DEFINITION( PreviewKeyDown, RoutingStrategy::Tunnel, UIElement, KeyEventArgs );
REGISTER_EVENT_DEFINITION( PreviewKeyUp, RoutingStrategy::Tunnel, UIElement, KeyEventArgs );


REGISTER_EVENT_DEFINITION( MouseUp, RoutingStrategy::Bubble, UIElement, MouseButtonEventArgs );
REGISTER_EVENT_DEFINITION( MouseDown, RoutingStrategy::Bubble, UIElement, MouseButtonEventArgs );

REGISTER_EVENT_DEFINITION( MouseRightButtonDown, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
REGISTER_EVENT_DEFINITION( MouseRightButtonUp, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
REGISTER_EVENT_DEFINITION( MouseLeftButtonDown, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
REGISTER_EVENT_DEFINITION( MouseLeftButtonUp, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );

REGISTER_EVENT_DEFINITION( MouseWheel, RoutingStrategy::Bubble, UIElement, MouseWheelEventArgs );
REGISTER_EVENT_DEFINITION( MouseMove, RoutingStrategy::Bubble, UIElement, MouseEventArgs );

REGISTER_EVENT_DEFINITION( KeyUp, RoutingStrategy::Tunnel, UIElement, KeyEventArgs );
REGISTER_EVENT_DEFINITION( KeyDown, RoutingStrategy::Tunnel, UIElement, KeyEventArgs );


REGISTER_EVENT_DEFINITION( MouseEnter, RoutingStrategy::Direct, UIElement, MouseEventArgs );
REGISTER_EVENT_DEFINITION( MouseLeave, RoutingStrategy::Direct, UIElement, MouseEventArgs );



// ================================ //
//
void		UIElement::SetParent				( UIElement* parent )
{
	m_parent = parent;
}

// ================================ //
//
const std::string&		UIElement::GetName		() const
{
	auto& name = GetHost()->GetControlName( this );
	return name;		// Note: We can return this->ToString() like in .NET but this would require to have ToString function in EngineObject
}

// ================================ //
//
HostWindow*				UIElement::GetHost		() const
{
	auto parent = GetParent();
	if( parent )
		return parent->GetHost();
	return nullptr;
}

// ================================ //
//
void		UIElement::InvokeEventDelegates		( EventType eventID, UIElement* sender, IEventArgs* e, AccessKey )
{
	auto container = m_eventHandlers.FindContainer( eventID );
	if( container )
		container->CallDelegates( sender, e );
}

}	// gui
}	// sw

