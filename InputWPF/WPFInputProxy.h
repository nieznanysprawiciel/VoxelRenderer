#pragma once
/**
@file WPFInputProxy.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "swInputLibrary/InputCore/IInput.h"

#include "swInputLibrary/InputCore/MouseDevice.h"
#include "swInputLibrary/InputCore/KeyboardDevice.h"
#include "swInputLibrary/InputCore/JoystickDevice.h"


namespace sw {
namespace input
{

/**@defgroup WPFInput
@ingroup Input*/



/**@brief Klasa do przekierowywania do silnika wejœcia pobranego z WPFa.

@ingroup WPFInput*/
class WPFInputProxy : public IInput
{
private:

	std::vector< const KeyboardState* >		m_keyboardsStates;	///< Copy of device state in m_keyboards.
	std::vector< const MouseState* >		m_mousesStates;		///< Copy of device state in m_mouses.
	std::vector< const JoystickState* >		m_joysticksStates;	///< Copy of device state in m_joysticks.

	std::vector< KeyboardDeviceOPtr >	m_keyboards;	///< Only first element is in use now.
	std::vector< MouseDeviceOPtr >		m_mouses;		///< Only first element is in use now.
	std::vector< JoystickDeviceOPtr >	m_joysticks;	///< Only first element is in use now.

	uint16			m_lastX;
	uint16			m_lastY;

	Timestamp		m_eventNum;							///< Number of event in current frame.

public:
	WPFInputProxy();
	~WPFInputProxy();

	virtual bool										Init				( const InputInitInfo& initInfo ) override;

	virtual const std::vector< const KeyboardState* >&	GetKeyboardStates	() const override;
	virtual const std::vector< const MouseState* >&		GetMouseStates		() const override;
	virtual const std::vector< const JoystickState* >&	GetJoystickStates	() const override;

	virtual std::vector< KeyboardDeviceOPtr >&			GetKeyboardDevice	() override;
	virtual std::vector< MouseDeviceOPtr >&				GetMouseDevice		() override;
	virtual std::vector< JoystickDeviceOPtr >&			GetJoystickDevice	() override;

	virtual std::vector< const InputDeviceInfo* >		GetDevicesInfo		() const override;

	virtual void										Update				( float timeInterval ) override;
	virtual bool										UpdateDevices		() override;

public:
	///@name WPF API
	///@{
	void				KeyboardChange				( int keyId, bool pressed );
	void				MouseButtonChange			( int button, bool pressed );
	void				MousePositionChange			( double X, double Y );
	void				MouseWheelChange			( double delta );

	void				LostFocus					();
	void				PostUpdate					();
	///@}
};



}	// input
}	// sw

