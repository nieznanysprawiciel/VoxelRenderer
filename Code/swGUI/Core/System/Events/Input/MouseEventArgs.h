#pragma once
/**
@file MouseEventArgs.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "InputEventArgs.h"

#include "swInputLibrary/InputCore/MouseDevice.h"



namespace sw {
namespace gui
{


/**@brief Base class for mouse input events.
@ingroup InputEvents*/
struct MouseEventArgs : public InputEventArgs
{
	RTTR_ENABLE( InputEventArgs )
public:

	input::MouseDevice*			Mouse;			///< Mouse device. You can get current mouse state here.
	const input::KeyState*		RightButton;	///< State of right button in moment of sending this event.
	const input::KeyState*		LeftButton;		///< State of left button in moment of sending this event.
	const input::KeyState*		MiddleButton;	///< State of middle button in moment of sending this event.
	const input::KeyState*		XButton1;		///< State of first extended button in moment of sending this event.
	const input::KeyState*		XButton2;		///< State of second extended button in moment of sending this event.

	float						WindowPosX;		///< Position of mouse in window space.
	float						WindowPosY;		///< Position of mouse in window space.

public:
	explicit MouseEventArgs( input::MouseDevice* device )
		:	InputEventArgs( device )
		,	Mouse( device )
		,	RightButton( device->GetState().RightButton() )
		,	LeftButton( device->GetState().LeftButton() )
		,	MiddleButton( device->GetState().MiddleButton() )
		,	XButton1( device->GetState().XButton1() )
		,	XButton2( device->GetState().XButton2() )
		,	WindowPosX( device->GetState().GetPositionX() )
		,	WindowPosY( device->GetState().GetPositionY() )
	{}
};

}	// gui
}	// sw

