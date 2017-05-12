#pragma once
/**
@file MouseMoveEventArgs.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "MouseEventArgs.h"

#include "swInputLibrary/InputCore/MouseDevice.h"



namespace sw {
namespace gui
{


/**@brief Mouse moved event.
@ingroup InputEvents*/
struct MouseMoveEventArgs : public MouseEventArgs
{
	RTTR_ENABLE( MouseEventArgs )
public:

	int16			MouseDeltaX;
	int16			MouseDeltaY;

public:

	explicit MouseMoveEventArgs( input::MouseDevice* device, int16 deltaX, int16 deltaY )
		:	MouseEventArgs( device )
		,	MouseDeltaX( deltaX )
		,	MouseDeltaY( deltaY )
	{}
};

DEFINE_OPTR_TYPE( MouseMoveEventArgs );

typedef EventDelegate< MouseMoveEventArgs > MouseMoveEventHandler;

}	// gui
}	// sw

