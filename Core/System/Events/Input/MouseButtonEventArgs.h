#pragma once
/**
@file MouseButtonEventArgs.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "MouseEventArgs.h"




namespace sw {
namespace gui
{


/**@brief Mouse button clicked event.
@ingroup InputEvents*/
struct MouseButtonEventArgs : public MouseEventArgs
{
	RTTR_ENABLE( MouseEventArgs )
public:

	const input::KeyState&				State;
	input::Mouse::PhysicalButtons		Button;
	bool								IsUp;
	bool								IsDown;

public:
	explicit	MouseButtonEventArgs( input::MouseDevice* device, input::Mouse::PhysicalButtons button )
		:	MouseEventArgs( device )
		,	Button( button )
		,	State( device->GetState()[ button ] )
		,	IsUp( device->GetState()[ button ].IsUp() )
		,	IsDown( device->GetState()[ button ].IsPressed() )
	{}
};

DEFINE_OPTR_TYPE( MouseButtonEventArgs );



}	// gui
}	// sw
