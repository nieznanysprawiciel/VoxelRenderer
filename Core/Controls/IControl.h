#pragma once

#include "swCommonLib/Common/EngineObject.h"
#include "swGUI/Core/System/CommonTypes/CommonTypes.h"

#include <string>


/**@defgroup EngineGUI
@ingroup ModulesStructure
@brief Modu³ s³u¿¹cy do renderowania interfejsu graficznego u¿ytkownika.*/

namespace GUI
{

struct IEventArgs;
struct InputEventArgs;
struct MouseEventArgs;
struct MouseButtonEventArgs;
struct MouseWheelEventArgs;
struct KeyEventArgs;

class HostWindow;
class TopLevelControl;



enum class VerticalAlignement : char
{
	Bottom,
	Center,
	Stretch,
	Top
};

enum class HorizontalAlignement : char
{
	Left,
	Center,
	Right,
	Stretch
};


/**@brief Klasa bazowa dla kontrolek.*/
class IControl : public EngineObject
{
	RTTR_ENABLE( EngineObject )
	RTTR_REGISTRATION_FRIEND
private:
protected:

	IControl*				m_parent;

	// Properties
	float					m_width;
	float					m_maxWidth;
	float					m_minWidth;
	float					m_height;
	float					m_maxHeight;
	float					m_minHeight;

	bool					m_isVisible : 1;
	bool					m_isEnabled : 1;
	bool					m_isMouseOver : 1;
	bool					m_isFocused : 1;
	bool					m_isFocusable : 1;

	VerticalAlignement		m_verticalAlignement;
	HorizontalAlignement	m_horizontalAlignement;

	Thickness				m_margin;

	EngineObject*			m_dataContext;

public:
							IControl		( IControl* parent );
	virtual					~IControl		();

	virtual bool			HitTest			( Position mouse, const Rect& parentArea );

	const std::string&		GetName			();

protected:

	IControl*				GetParent		()			{ return m_parent; }
	TopLevelControl*		GetRootControl	();
	HostWindow*				GetHost			();

public:
	// Event handlers

	virtual void			Loaded						( IControl* sender, IEventArgs* e )					{}
	virtual void			Unloaded					( IControl* sender, IEventArgs* e )					{}

	virtual void			GotFocus					( IControl* sender, IEventArgs* e )					{}
	virtual void			LostFocus					( IControl* sender, IEventArgs* e )					{}

	// Mouse
	virtual void			MouseEnter					( IControl* sender, MouseEventArgs* e )				{}
	virtual void			MouseLeave					( IControl* sender, MouseEventArgs* e )				{}
	virtual void			MouseMove					( IControl* sender, MouseEventArgs* e )				{}
	virtual void			MouseWheel					( IControl* sender, MouseWheelEventArgs* e )		{}

	virtual void			MouseDown					( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			MouseUp						( IControl* sender, MouseButtonEventArgs* e )		{}

	virtual void			MouseRightButtonDown		( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			MouseRightButtonUp			( IControl* sender, MouseButtonEventArgs* e )		{}

	virtual void			MouseLeftButtonDown			( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			MouseLeftButtonUp			( IControl* sender, MouseButtonEventArgs* e )		{}


	virtual void			PreviewMouseEnter				( IControl* sender, MouseEventArgs* e )				{}
	virtual void			PreviewMouseLeave				( IControl* sender, MouseEventArgs* e )				{}
	virtual void			PreviewMouseMove				( IControl* sender, MouseEventArgs* e )				{}
	virtual void			PreviewMouseWheel				( IControl* sender, MouseWheelEventArgs* e )		{}

	virtual void			PreviewMouseDown				( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			PreviewMouseUp					( IControl* sender, MouseButtonEventArgs* e )		{}

	virtual void			PreviewMouseRightButtonDown		( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			PreviewMouseRightButtonUp		( IControl* sender, MouseButtonEventArgs* e )		{}

	virtual void			PreviewMouseLeftButtonDown		( IControl* sender, MouseButtonEventArgs* e )		{}
	virtual void			PreviewMouseLeftButtonUp		( IControl* sender, MouseButtonEventArgs* e )		{}

	//Keyboard
	virtual void			KeyDown							( IControl* sender, KeyEventArgs* e )				{}
	virtual void			KeyUp							( IControl* sender, KeyEventArgs* e )				{}

	virtual void			PreviewKeyDown					( IControl* sender, KeyEventArgs* e )				{}
	virtual void			PreviewKeyUp					( IControl* sender, KeyEventArgs* e )				{}


};

}	// GUI
