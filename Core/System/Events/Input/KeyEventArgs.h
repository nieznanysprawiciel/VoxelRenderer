#pragma once
/**
@file KeyEventArgs.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "swGUI/Core/System/Events/Input/KeyboardEventArgs.h"


namespace sw {
namespace gui
{


/**@brief Key state changed event.

@ingroup InputEvents*/
struct KeyEventArgs : public KeyboardEventArgs
{
	RTTR_ENABLE( KeyboardEventArgs )
public:

	const input::KeyState&				State;
	input::Keyboard::PhysicalKeys		Key;
	bool								IsUp;
	bool								IsDown;

public:

	KeyEventArgs( input::KeyboardDevice* device, input::Keyboard::PhysicalKeys key )
		:	KeyboardEventArgs( device )
		,	State( device->GetState()[ key ] )
		,	Key( key )
		,	IsUp( device->GetState()[ key ].IsUp() )
		,	IsDown( device->GetState()[ key ].IsPressed() )
	{}

};
DEFINE_OPTR_TYPE( KeyEventArgs );


}	// gui
}	// sw
