#pragma once
/**
@file DebugInput.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swInputLibrary/InputCore/IInput.h"

#include "swInputLibrary/InputCore/Debugging/EventCapture.h"


namespace sw {
namespace input
{

/**@defgroup DebugInput
@brief Classes created for test purposes.
You can use it to load bunch of events from disk, write them from code
@ingroup Input*/


/**@brief Input class for testing and debugging.

This class can fake input by reading events form @ref EventCapture object.
@ingroup DebugInput*/
class DebugInput : public IInput
{
private:

	std::vector< const KeyboardState* >		m_keyboardsStates;	///< Copy of device state in m_keyboards.
	std::vector< const MouseState* >		m_mousesStates;		///< Copy of device state in m_mouses.
	std::vector< const JoystickState* >		m_joysticksStates;	///< Copy of device state in m_joysticks.

	std::vector< KeyboardDeviceOPtr >	m_keyboards;	///< Only first element is in use now.
	std::vector< MouseDeviceOPtr >		m_mouses;		///< Only first element is in use now.
	std::vector< JoystickDeviceOPtr >	m_joysticks;	///< Only first element is in use now.

	Timestamp		m_eventNum;							///< Number of event in current frame.
	Size			m_frameNumber;

	EventCapture*	m_eventCapture;

protected:
public:
	explicit		DebugInput		() = default;
	virtual			~DebugInput		() = default;


	virtual bool											Init				( const InputInitInfo & initInfo ) override;

	virtual const std::vector< const KeyboardState* >&		GetKeyboardStates	() const override;
	virtual const std::vector< const MouseState* >&			GetMouseStates		() const override;
	virtual const std::vector< const JoystickState* >&		GetJoystickStates	() const override;

	virtual std::vector< KeyboardDeviceOPtr >&				GetKeyboardDevice	() override;
	virtual std::vector< MouseDeviceOPtr >&					GetMouseDevice		() override;
	virtual std::vector< JoystickDeviceOPtr >&				GetJoystickDevice	() override;

	virtual std::vector< const InputDeviceInfo* >			GetDevicesInfo		() const override;

	virtual void			Update					( float timeInterval )	override;
	virtual bool			UpdateDevices			()						override;

};


}	// input
}	// sw
