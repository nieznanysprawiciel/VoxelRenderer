#pragma once
/**
@file JoystickState.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "InputDeviceInfo.h"


/**@brief Stan joysticka lub innego urz¹dzenia wejœcia, które nie jest
klawiatur¹ lub mysz¹.*/
class JoystickState
{
private:

	InputDeviceInfo		m_info;


public:
	JoystickState();
	~JoystickState();

	const InputDeviceInfo&		GetInfo()	{ return m_info; }
};

inline JoystickState::JoystickState()
{ }


inline JoystickState::~JoystickState()
{ }


