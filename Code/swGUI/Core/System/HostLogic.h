#pragma once
/**
@file HostLogic.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swGUI/Core/Controls/UIElement.h"

#include <vector>
#include <map>



namespace sw {
namespace gui
{

class HostWindow;


/**@brief Logic of host window for managing child controls.

@ingroup ControlsFramework*/
class HostLogic
{
	friend class HostLogicTester;
private:

	std::vector< UIElement* >	m_controlsTree;		///< Top level controls. This vector holds entire tree.

	std::vector< UIElement* >	m_mousePath;		///< Controls hierarchy that have mouse over in this frame.
	std::vector< UIElement* >	m_keyboardFocus;	///< Path of controls that have keyboard focus and all events are directed to them.
	UIElement*					m_mouseCapture;		///< Element that captured mouse. Can be nullptr.

	std::vector< UIElement* >	m_invalidated;		///< Controls which needs to be redrawn in this frame. @todo Move to different logic. Separate rearrangement and redraw.
	

	///@name Controls info
	///@{

	/// Map containing windows names. Most controls don't have name, so it's better to store
	/// them separatly, to lower memory consumption.
	std::map< const UIElement*, std::string >		m_controlsNames;

	///@}

protected:
public:
	explicit		HostLogic		( HostWindow* hostWindow );
					~HostLogic		() = default;


	void				RemoveControl		( UIElement* control );

	void				RegisterControlName	( const UIElement* control, const std::string& name );
	const std::string&	GetControlName		( const UIElement* control ) const;


	///@name GUI system interaction
	///@{
	void				LostFocus		();
	void				GotFocus		();

	void				OnResized		( uint16 newWidth, uint16 newHeight );
	void				OnMaximized		();
	void				OnMinimized		();
	///@}


public:
	///@name Input handling
	///@{
	void				HandleKeyInput			( const input::DeviceEvent& event, input::Device* device );
	void				HandleCharInput			( const input::DeviceEvent& event, input::Device* device );
	void				HandleMouseButtonInput	( const input::DeviceEvent& event, input::Device* device );
	void				HandleMouseWheelInput	( const input::DeviceEvent& event, input::Device* device );
	void				HandleMouseMoveInput	( const input::DeviceEvent& event, input::Device* device );
	///@}

private:

	UIElement*			HitTesting				();

public:

	Size				GetMemorySize		();
};



}	// gui
}	// sw

