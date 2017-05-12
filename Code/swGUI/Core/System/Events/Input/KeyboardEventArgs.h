#pragma once

#include "swGUI/Core/System/Events/Input/InputEventArgs.h"

#include "swInputLibrary/InputCore/KeyboardDevice.h"


namespace sw {
namespace gui
{


/**@brief Base class for keyboard events.
@ingroup InputEvents*/
struct KeyboardEventArgs : public InputEventArgs
{
	RTTR_ENABLE( InputEventArgs )
public:

	input::KeyboardDevice*			Keyboard;


public:
	explicit KeyboardEventArgs( input::KeyboardDevice* keyboard )
		:	InputEventArgs( keyboard )
		,	Keyboard( keyboard )
	{}

};

}	// gui
}	// sw
