#pragma once
/**
@file WinaApiInputProxy.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "swInputLibrary/InputCore/IInput.h"

#include "swInputLibrary/InputCore/MouseDevice.h"
#include "swInputLibrary/InputCore/KeyboardDevice.h"
#include "swInputLibrary/InputCore/JoystickDevice.h"


#include <Windows.h>



namespace sw {
namespace input
{

/**@defgroup WinAPIInput
@ingroup Input*/


/**@brief Klasa do przekierowywania do silnika wejœcia pobranego z WPFa.

@ingroup WinAPIInput*/
class WinApiInputProxy : public IInput
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
	WinApiInputProxy();
	~WinApiInputProxy();

	virtual bool									Init				( const InputInitInfo& initInfo ) override;

	virtual const std::vector< const KeyboardState* >&	GetKeyboardStates	() const override;
	virtual const std::vector< const MouseState* >&		GetMouseStates		() const override;
	virtual const std::vector< const JoystickState* >&	GetJoystickStates	() const override;

	virtual std::vector< KeyboardDeviceOPtr >&		GetKeyboardDevice	() override;
	virtual std::vector< MouseDeviceOPtr >&			GetMouseDevice		() override;
	virtual std::vector< JoystickDeviceOPtr >&		GetJoystickDevice	() override;

	virtual std::vector< const InputDeviceInfo* >	GetDevicesInfo		() const override;

	virtual void									Update				( float timeInterval ) override;
	virtual bool									UpdateDevices		() override;

public:
	///@name WinaAPI API
	///@{
	void				KeyboardChange				( int keyId, bool pressed );
	void				KeyboardCharacter			( wchar_t character );
	void				MouseButtonChange			( int button, bool pressed );
	void				MousePositionChange			( short X, short Y );
	void				MouseWheelChange			( double delta );

	void				LostFocus					();

	void				HandleEvent					( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	///@}
};



}	// input
}	// sw

