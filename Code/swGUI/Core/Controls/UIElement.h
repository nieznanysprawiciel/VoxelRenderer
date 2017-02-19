#pragma once
/**
@file UIElement.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "swGUI/Core/Controls/Visual.h"



namespace sw {
namespace gui
{

class UIElement;

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

Connects WPF UIElement and FrameworkElement classes.*/
class UIElement : public Visual
{
	RTTR_ENABLE( Visual )
		RTTR_REGISTRATION_FRIEND
private:
protected:

	UIElement*				m_parent;				///< Parent in controls visual tree.

	bool					m_isVisible : 1;
	bool					m_isEnabled : 1;	///< @todo Maybe it's something from higher level.
	bool					m_isMouseOver : 1;
	bool					m_isFocused : 1;
	bool					m_isFocusable : 1;

	Size2D					m_desiredSize;			///< Desired size after Messure pass.



public:
	explicit		UIElement	() = default;
	~UIElement	() = default;


///@name Layout fuctions
///@{
/**@brief Meassure pass. Check documentation in WPF.*/
	virtual Size2D			Measure				( Size2D availableSize )		= 0;
	/**@brief Arragement pass. Check WPF documentation.*/
	virtual void			Arrange				( Rect& finalRect )				= 0;
	///@}


	/**@brief Get number of children in visual tree.*/
	virtual Size			GetNumChildren		()								= 0;
	/**@brief Gets child in visual tree.*/
	virtual UIElement*		GetUIChild			( Size idx )					= 0;

public:

	///@name GUISystem interaction events
	///Mouse and Keyboard input events and focus.
	///@{
	virtual void			Loaded						( UIElement* sender, IEventArgs* e ) { }
	virtual void			Unloaded					( UIElement* sender, IEventArgs* e ) { }

	virtual void			GotFocus					( UIElement* sender, IEventArgs* e ) { }
	virtual void			LostFocus					( UIElement* sender, IEventArgs* e ) { }

	// Mouse
	virtual void			MouseEnter					( UIElement* sender, MouseEventArgs* e ) { }
	virtual void			MouseLeave					( UIElement* sender, MouseEventArgs* e ) { }
	virtual void			MouseMove					( UIElement* sender, MouseEventArgs* e ) { }
	virtual void			MouseWheel					( UIElement* sender, MouseWheelEventArgs* e ) { }

	virtual void			MouseDown					( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			MouseUp						( UIElement* sender, MouseButtonEventArgs* e ) { }

	virtual void			MouseRightButtonDown		( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			MouseRightButtonUp			( UIElement* sender, MouseButtonEventArgs* e ) { }

	virtual void			MouseLeftButtonDown			( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			MouseLeftButtonUp			( UIElement* sender, MouseButtonEventArgs* e ) { }


	virtual void			PreviewMouseEnter				( UIElement* sender, MouseEventArgs* e ) { }
	virtual void			PreviewMouseLeave				( UIElement* sender, MouseEventArgs* e ) { }
	virtual void			PreviewMouseMove				( UIElement* sender, MouseEventArgs* e ) { }
	virtual void			PreviewMouseWheel				( UIElement* sender, MouseWheelEventArgs* e ) { }

	virtual void			PreviewMouseDown				( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			PreviewMouseUp					( UIElement* sender, MouseButtonEventArgs* e ) { }

	virtual void			PreviewMouseRightButtonDown		( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			PreviewMouseRightButtonUp		( UIElement* sender, MouseButtonEventArgs* e ) { }

	virtual void			PreviewMouseLeftButtonDown		( UIElement* sender, MouseButtonEventArgs* e ) { }
	virtual void			PreviewMouseLeftButtonUp		( UIElement* sender, MouseButtonEventArgs* e ) { }

	//Keyboard
	virtual void			KeyDown							( UIElement* sender, KeyEventArgs* e ) { }
	virtual void			KeyUp							( UIElement* sender, KeyEventArgs* e ) { }

	virtual void			PreviewKeyDown					( UIElement* sender, KeyEventArgs* e ) { }
	virtual void			PreviewKeyUp					( UIElement* sender, KeyEventArgs* e ) { }
	///@}
};



}	// gui
}	// sw

