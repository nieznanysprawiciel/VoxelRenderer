/**
@file HostLogic.coo
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "HostLogic.h"


#include "CommonTypes/CommonTypes.h"

#include "HostWindow.h"

// Events system.
#include "Events/EventsSystem.h"
#include "Events/EventsSystem.inl"

// Events arguments structures.
#include "Events/Input/KeyEventArgs.h"



// One pointer at least, but I don't know how much it needs in reality
#define STD_MAP_OVERHEAD_PER_ELEMENT 8


namespace sw {
namespace gui
{


// ================================ //
//
HostLogic::HostLogic	( HostWindow* hostWindow )
{
	m_mouseCapture = hostWindow;
	m_keyboardFocus.push_back( hostWindow );
}


// ================================ //
//
void					HostLogic::RemoveControl		( UIElement* control )
{}

// ================================ //
//
void					HostLogic::RegisterControlName	( const UIElement* control, const std::string& name )
{
	assert( m_controlsNames.find( control ) == m_controlsNames.end() );
	m_controlsNames[ control ] = name;
}

// ================================ //
//
const std::string&		HostLogic::GetControlName	( const UIElement* control ) const
{
	auto iter = m_controlsNames.find( control );
	if( iter != m_controlsNames.end() )
		return iter->second;
	else
		return EMPTY_STRING;
}

//====================================================================================//
//			GUI system interactions	
//====================================================================================//


// ================================ //
//
void			HostLogic::LostFocus		()
{}

// ================================ //
//
void			HostLogic::GotFocus			()
{}

// ================================ //
//
void			HostLogic::OnResized		( uint16 newWidth, uint16 newHeight )
{}

// ================================ //
//
void			HostLogic::OnMaximized		()
{}

// ================================ //
//
void			HostLogic::OnMinimized		()
{}


//====================================================================================//
//			Input handling	
//====================================================================================//

// ================================ //
//
template< typename ArgumentType >
void				SendBubbleTunnelEventsPair			( UIElement* element,
														  ArgumentType* event,
														  const RegisteredEvent* bubble,
														  const RegisteredEvent* tunnel,
														  EventHandlerPointer< ArgumentType > bubbleHandler,
														  EventHandlerPointer< ArgumentType > tunnelHandler )
{
	bool result = EventsSystem::Get().RaiseEvent( bubble, element, event, bubbleHandler );
	assert( result );		/// @todo Handle failing during event raising.

	if( !event->Handled )
	{
		result = EventsSystem::Get().RaiseEvent( tunnel, element, event, tunnelHandler );
		assert( result );	/// @todo Handle failing during event raising.
	}
}

// ================================ //
//
void				HostLogic::HandleKeyInput			( const input::DeviceEvent& event, input::Device* device )
{
	// Send event to UIElement that has keyboard focus.
	if( m_keyboardFocus.size() )
	{
		UIElement* bottomElement = m_keyboardFocus.back();
		KeyEventArgsOPtr keyEvent = KeyEventArgsOPtr( new KeyEventArgs( static_cast< input::KeyboardDevice* >( device ), event.Key.Key ) );

		if( keyEvent->IsUp )
			SendBubbleTunnelEventsPair( bottomElement, keyEvent.get(), UIElement::sPreviewKeyUp, UIElement::sKeyUp, &UIElement::OnPreviewKeyUp, &UIElement::OnKeyUp );
		else
			SendBubbleTunnelEventsPair( bottomElement, keyEvent.get(), UIElement::sPreviewKeyDown, UIElement::sKeyDown, &UIElement::OnPreviewKeyDown, &UIElement::OnKeyDown );
	}
}

// ================================ //
//
void				HostLogic::HandleCharInput			( const input::DeviceEvent& event, input::Device* device )
{}

// ================================ //
//
void				HostLogic::HandleMouseButtonInput	( const input::DeviceEvent& event, input::Device* device )
{
	UIElement* target = nullptr;

	if( m_mouseCapture )
		target = m_mouseCapture;
	else if( !m_mousePath.empty() )
		target = m_mousePath.back();

	if( target )
	{
		MouseButtonEventArgsOPtr mouseEvent = MouseButtonEventArgsOPtr( new MouseButtonEventArgs( static_cast< input::MouseDevice* >( device ), event.Button.Button ) );

		if( mouseEvent->IsUp )
			SendBubbleTunnelEventsPair( target, mouseEvent.get(), UIElement::sPreviewMouseUp, UIElement::sMouseUp, &UIElement::OnPreviewMouseUp, &UIElement::OnMouseUp );
		else
			SendBubbleTunnelEventsPair( target, mouseEvent.get(), UIElement::sPreviewMouseDown, UIElement::sMouseDown, &UIElement::OnPreviewMouseDown, &UIElement::OnMouseDown );
	}
}

// ================================ //
//
void				HostLogic::HandleMouseWheelInput	( const input::DeviceEvent& event, input::Device* device )
{}

// ================================ //
//
void				HostLogic::HandleMouseMoveInput		( const input::DeviceEvent& event, input::Device* device )
{
	UIElement* target = nullptr;

	if( m_mouseCapture )
		target = m_mouseCapture;
	else if( !m_mousePath.empty() )
		target = HitTesting();
	

	if( target )
	{
		MouseMoveEventArgsOPtr mouseEvent = MouseMoveEventArgsOPtr( new MouseMoveEventArgs( static_cast< input::MouseDevice* >( device ), event.Cursor.OffsetX, event.Cursor.OffsetY ) );
		SendBubbleTunnelEventsPair( target, mouseEvent.get(), UIElement::sPreviewMouseMove, UIElement::sMouseMove, &UIElement::OnPreviewMouseMove, &UIElement::OnMouseMove );
	}
}


// ================================ //
//
UIElement*			HostLogic::HitTesting				()
{
	// Check if mouse is still in m_mousePath. Change this vector if not. Send MouseLeave and MouseEnter
	// events to all windows that lost mouse or which were entered by mouse.

	return nullptr;
}

//====================================================================================//
//			Additional functionalities	
//====================================================================================//


// ================================ //
//
Size					HostLogic::GetMemorySize()
{
	Size size = sizeof( HostLogic );

	size += m_mousePath.capacity() * sizeof( UIElement* );
	size += m_invalidated.capacity() * sizeof( UIElement* );
	size += m_controlsTree.capacity() * sizeof( UIElement* );

	size += m_controlsNames.size() * ( sizeof( std::pair< UIElement*, std::string > ) + STD_MAP_OVERHEAD_PER_ELEMENT );

	//size += m_nativeWindow->MemorySize();

	for( auto control : m_controlsTree )
		size += control->MemorySize();

	return size;
}

}	// gui
}	// sw
