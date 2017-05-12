#pragma once
/**
@file MouseWheelEventArgs.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "MouseEventArgs.h"


namespace sw {
namespace gui
{


/**@brief Mouse wheel changed position.
@ingroup InputEvents*/
struct MouseWheelEventArgs : public MouseEventArgs
{
	RTTR_ENABLE( MouseEventArgs )
public:

	float				Delta;

public:
	explicit MouseWheelEventArgs( input::MouseDevice* device )
		:	MouseEventArgs( device )
		,	Delta( device->GetState().WheelDelta() )
	{}

};

}	// gui
}	// sw
