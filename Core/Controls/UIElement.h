#pragma once
/**
@file UIElement.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "swGUI/Core/Controls/Visual.h"
#include "swGUI/Core/System/Events/EventHandlers.h"
#include "swGUI/Core/System/Events/EventsSystem.h"

// Include events arguments
#include "swGUI/Core/System/Events/Input/MouseButtonEventArgs.h"
#include "swGUI/Core/System/Events/Input/MouseMoveEventArgs.h"
#include "swGUI/Core/System/Events/Input/MouseWheelEventArgs.h"
#include "swGUI/Core/System/Events/Input/KeyEventArgs.h"



/**@defgroup ControlsFramework Controls Framework
@brief Base control classes and controls processing framework.
@ingroup GUICore*/



namespace sw {
namespace gui
{

class HostWindow;
class UIElement;
DEFINE_OPTR_TYPE( UIElement );

struct IEventArgs;
struct InputEventArgs;
struct MouseEventArgs;
struct MouseButtonEventArgs;
struct MouseWheelEventArgs;
struct KeyEventArgs;



/**@brief Interface for all controls in tree.

Provides functionalities:
- Input interaction and events
- Layout (Measure/Arrange)
- Managing focus
- Parent/Child relationship

Connects WPF UIElement and FrameworkElement classes.

@todo Implement logical focus in future. (Check in WPF)

@ingroup ControlsFramework*/
class UIElement : public Visual
{
	RTTR_ENABLE( Visual );
	RTTR_REGISTRATION_FRIEND;

	friend class UIElementTester;
private:
protected:

	UIElement*				m_parent;				///< Parent in controls visual tree.

	bool					m_isVisible : 1;
	bool					m_isEnabled : 1;				///< @todo Maybe it's something from higher level.
	bool					m_isMouseOver : 1;
	bool					m_isFocused : 1;				///< This is logical focus.
	bool					m_isKeyboardFocus : 1;			///< This element has keyboard focus and will receive keyboard events.
	bool					m_isKeyboardFocusWithin : 1;	///< This element or it's children have keyboard focus.
	bool					m_isMouseCaptured : 1;			///< Check if element have captured mouse.
	bool					m_isMouseCapturedWithin : 1;	///< Element or it's children have captured mouse.
	bool					m_isFocusable : 1;

	Size2D					m_desiredSize;			///< Desired size after Messure pass.
	EventHandlers			m_eventHandlers;		///< Managing delegates.

public:
	explicit		UIElement	() = default;
	virtual			~UIElement	() = default;


	///@name Layout fuctions
	///@{
	/**@brief Meassure pass. Check documentation in WPF.*/
	virtual Size2D			Measure				( Size2D availableSize )		= 0;
	/**@brief Arragement pass. Check WPF documentation.*/
	virtual void			Arrange				( Rect& finalRect )				= 0;
	///@}


	///@name Visual tree manipulation
	///@{
	/**@brief Get number of children in visual tree.*/
	virtual Size			GetNumChildren		() const						= 0;
	
	/**@brief Gets child in visual tree.*/
	virtual UIElement*		GetUIChild			( Size idx ) const				= 0;
	
	/**@brief Add child.
	This function should set child parent to this pointer.
	@return Returns false if this control can't have children.*/
	virtual bool			AddChild			( UIElementOPtr&& child )		= 0;

	/**@brief Set control parent.
	Internal use.*/
	virtual void			SetParent			( UIElement* parent );

	/**@brief Returns control parent.*/
	virtual UIElement*		GetParent			() const { return m_parent; }

	///@}

	virtual const std::string&		GetName		() const;
	virtual HostWindow*				GetHost		() const;
public:

	///@name Restricted
	///@{
	virtual	void			InvokeEventDelegates		( EventType eventID, UIElement* sender, IEventArgs* e, AccessKey );

	///@}


	///@name GUISystem interaction events
	///Mouse and Keyboard input events and focus.
	///@{
	virtual void			OnLoaded					( UIElement* sender, IEventArgs* e ) { }
	virtual void			OnUnloaded					( UIElement* sender, IEventArgs* e ) { }

	virtual void			OnGotFocus					( UIElement* sender, IEventArgs* e ) { }
	virtual void			OnLostFocus					( UIElement* sender, IEventArgs* e ) { }

	// Mouse
	virtual void			OnMouseEnter				( UIElement* sender, MouseEventArgs* e ) { }
	virtual void			OnMouseLeave				( UIElement* sender, MouseEventArgs* e ) { }
	virtual void			OnMouseMove					( UIElement* sender, MouseMoveEventArgs* e ) { }
	virtual void			OnMouseWheel				( UIElement* sender, MouseWheelEventArgs* e ) { }

	virtual void			OnMouseDown					( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			OnMouseUp					( UIElement* sender, MouseButtonEventArgs* e ) { }

	virtual void			OnMouseRightButtonDown		( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			OnMouseRightButtonUp		( UIElement* sender, MouseButtonEventArgs* e ) { }

	virtual void			OnMouseLeftButtonDown		( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			OnMouseLeftButtonUp			( UIElement* sender, MouseButtonEventArgs* e ) { }


	virtual void			OnPreviewMouseMove				( UIElement* sender, MouseMoveEventArgs* e ) { }
	virtual void			OnPreviewMouseWheel				( UIElement* sender, MouseWheelEventArgs* e ) { }

	virtual void			OnPreviewMouseDown				( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			OnPreviewMouseUp				( UIElement* sender, MouseButtonEventArgs* e ) { }

	virtual void			OnPreviewMouseRightButtonDown	( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			OnPreviewMouseRightButtonUp		( UIElement* sender, MouseButtonEventArgs* e ) { }

	virtual void			OnPreviewMouseLeftButtonDown	( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			OnPreviewMouseLeftButtonUp		( UIElement* sender, MouseButtonEventArgs* e ) { }

	//Keyboard
	virtual void			OnKeyDown						( UIElement* sender, KeyEventArgs* e ) { }
	virtual void			OnKeyUp							( UIElement* sender, KeyEventArgs* e ) { }

	virtual void			OnPreviewKeyDown				( UIElement* sender, KeyEventArgs* e ) { }
	virtual void			OnPreviewKeyUp					( UIElement* sender, KeyEventArgs* e ) { }
	///@}


public:

	//
	REGISTER_EVENT_DECLARATION( PreviewMouseUp, RoutingStrategy::Tunnel, UIElement, MouseButtonEventArgs );
	REGISTER_EVENT_DECLARATION( PreviewMouseDown, RoutingStrategy::Tunnel, UIElement, MouseButtonEventArgs );

	REGISTER_EVENT_DECLARATION( PreviewMouseRightButtonUp, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
	REGISTER_EVENT_DECLARATION( PreviewMouseRightButtonDown, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
	REGISTER_EVENT_DECLARATION( PreviewMouseLeftButtonDown, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
	REGISTER_EVENT_DECLARATION( PreviewMouseLeftButtonUp, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );

	REGISTER_EVENT_DECLARATION( PreviewMouseWheel, RoutingStrategy::Tunnel, UIElement, MouseWheelEventArgs );
	REGISTER_EVENT_DECLARATION( PreviewMouseMove, RoutingStrategy::Tunnel, UIElement, MouseMoveEventArgs );

	REGISTER_EVENT_DECLARATION( PreviewKeyDown, RoutingStrategy::Tunnel, UIElement, KeyEventArgs );
	REGISTER_EVENT_DECLARATION( PreviewKeyUp, RoutingStrategy::Tunnel, UIElement, KeyEventArgs );

	//
	REGISTER_EVENT_DECLARATION( MouseUp, RoutingStrategy::Bubble, UIElement, MouseButtonEventArgs );
	REGISTER_EVENT_DECLARATION( MouseDown, RoutingStrategy::Bubble, UIElement, MouseButtonEventArgs );

	REGISTER_EVENT_DECLARATION( MouseRightButtonDown, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
	REGISTER_EVENT_DECLARATION( MouseRightButtonUp, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
	REGISTER_EVENT_DECLARATION( MouseLeftButtonDown, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );
	REGISTER_EVENT_DECLARATION( MouseLeftButtonUp, RoutingStrategy::Direct, UIElement, MouseButtonEventArgs );

	REGISTER_EVENT_DECLARATION( MouseWheel, RoutingStrategy::Bubble, UIElement, MouseWheelEventArgs );
	REGISTER_EVENT_DECLARATION( MouseMove, RoutingStrategy::Bubble, UIElement, MouseMoveEventArgs );

	REGISTER_EVENT_DECLARATION( KeyUp, RoutingStrategy::Tunnel, UIElement, KeyEventArgs );
	REGISTER_EVENT_DECLARATION( KeyDown, RoutingStrategy::Tunnel, UIElement, KeyEventArgs );

	//
	REGISTER_EVENT_DECLARATION( MouseEnter, RoutingStrategy::Direct, UIElement, MouseEventArgs );
	REGISTER_EVENT_DECLARATION( MouseLeave, RoutingStrategy::Direct, UIElement, MouseEventArgs );
};



}	// gui
}	// sw

