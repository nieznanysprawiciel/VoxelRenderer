#pragma once
/**
@file JoystickState.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/




namespace sw {
namespace input
{


/**@brief State of joystick device or other controllers which aren't mouse and keyboard.

@ingroup Input*/
class JoystickState
{
private:

public:
	JoystickState();
	~JoystickState();


	void		RemoveEvents	();
};

// ================================ //
//
inline JoystickState::JoystickState()
{}

// ================================ //
//
inline JoystickState::~JoystickState()
{}


// ================================ //
//
inline void			JoystickState::RemoveEvents	()
{}

}	// input
}	// sw

