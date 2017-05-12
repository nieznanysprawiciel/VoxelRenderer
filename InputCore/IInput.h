#pragma once
/**
@file IInput.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "KeyboardState.h"
#include "MouseState.h"
#include "JoystickState.h"

#include "swInputLibrary/InputCore/MouseDevice.h"
#include "swInputLibrary/InputCore/KeyboardDevice.h"
#include "swInputLibrary/InputCore/JoystickDevice.h"

#include "InputDeviceInfo.h"

#include <vector>

typedef void*	AppInstanceHandle;
typedef void*	WindowHandle;

namespace sw {
namespace input
{


/**@defgroup Input
@brief Sleeping Wombat input library.

This library provides abstraction layer for native OS input like WinAPI, WPF, DirectInput.
Library produces input events and state in system independent manner.

@ingroup ModulesStructure*/



class EventCapture;


/**@brief Initialization structure for input objects.

@ingroup Input*/
struct InputInitInfo
{
	AppInstanceHandle		AppInstance;
	WindowHandle			WndHandle;
	EventCapture*			EventCapturer;
};



/**@brief Interface for input classes for capturing user input.

@todo Deal with situation when we must have two different IInput objects to handle input. For example
WinAPI deals with keyboard and mouse and other IINput with Joysticks.

@todo We must think about handling other devices. For example WPF have such types as StylusDevice, TouchDevice and TabletDevice.
Can we do it as combinations of keyboard, mouse and joysticks or we must introduce new API for that.

@todo DirectInput should be created one per window. In this moment we have one input in whole application. We must rethink it.

@ingroup Input*/
class IInput
{
private:
protected:
public:
	virtual				~IInput() = 0 {};

	/**@brief Inicjalizuje ca³y modu³. Po wywo³aniu tej funkcji modu³ musi byæ gotowy do u¿ytku
	w innym przypadku powinien zwróciæ false.*/
	virtual bool										Init				( const InputInitInfo& initInfo ) = 0;

	virtual const std::vector< const KeyboardState* >&	GetKeyboardStates	() const = 0;
	virtual const std::vector< const MouseState* >&		GetMouseStates		() const = 0;
	virtual const std::vector< const JoystickState* >&	GetJoystickStates	() const = 0;

	virtual std::vector< KeyboardDeviceOPtr >&			GetKeyboardDevice	() = 0;
	virtual std::vector< MouseDeviceOPtr >&				GetMouseDevice		() = 0;
	virtual std::vector< JoystickDeviceOPtr >&			GetJoystickDevice	() = 0;

	virtual std::vector< const InputDeviceInfo* >		GetDevicesInfo		() const = 0;

	virtual void										Update				( float timeInterval ) = 0;

	/**@brief Funkcja na nowo sprawdza obecne w systemie urz¹dzenia i aktualizuje wszystkie
	potrzebne dane.*/
	virtual bool										UpdateDevices		() = 0;
};




}	// input
}	// sw

