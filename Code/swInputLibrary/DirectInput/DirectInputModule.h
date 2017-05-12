#pragma once

#include "swInputLibrary/InputCore/IInput.h"
#include "IncludeDirectInput.h"

#include <vector>



namespace sw {
namespace input
{



/**@defgroup DirectInput
@ingroup Input*/


/**@brief Klasa obs³uguj¹ca wejœcie u¿ytkownika.

U¿ywa interfejsu Direct Inputa.

@todo Zrobiæ obs³ugê wielu urz¹dzeñ tego samego typu i jousticka.
@todo Pozbyæ siê includowania direct input z EngineCore.

@ingroup DirectInput*/
class DirectInputModule : public IInput
{
private:

	HWND						m_windowHandle;

	LPDIRECTINPUT8				m_directInput;
	LPDIRECTINPUTDEVICE8		m_keyboardInput;		///<@todo Pewnie to powienien byæ wektor.
	LPDIRECTINPUTDEVICE8		m_mouseInput;			///<@todo Pewnie to powienien byæ wektor.

	std::vector< const KeyboardState* >		m_keyboardsStates;	///< Copy of device state in m_keyboards.
	std::vector< const MouseState* >		m_mousesStates;		///< Copy of device state in m_mouses.
	std::vector< const JoystickState* >		m_joysticksStates;	///< Copy of device state in m_joysticks.

	std::vector< KeyboardDeviceOPtr >	m_keyboards;	///< Only first element is in use now.
	std::vector< MouseDeviceOPtr >		m_mouses;		///< Only first element is in use now.
	std::vector< JoystickDeviceOPtr >	m_joysticks;	///< Only first element is in use now.

public:
	explicit										DirectInputModule	();
	virtual											~DirectInputModule	();

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

private:

	void			CleanDirectInput	();

	void			UpdateKeyboard		( int idx );
	void			UpdateMouse			( int idx );
	void			UpdateJoystick		( int idx );
};


}	// input
}	// sw
