#pragma once

#include "swInputLibrary/IInput.h"
#include "IncludeDirectInput.h"

#include <vector>


/**@brief Klasa obs³uguj¹ca wejœcie u¿ytkownika.

U¿ywa interfejsu Direct Inputa.

@todo Zrobiæ obs³ugê wielu urz¹dzeñ tego samego typu i jousticka.
@todo Pozbyæ siê includowania direct input z EngineCore.*/
class DirectInputModule : public IInput
{
private:

	HWND						m_windowHandle;

	LPDIRECTINPUT8				m_directInput;
	LPDIRECTINPUTDEVICE8		m_keyboardInput;		///<@todo Pewnie to powienien byæ wektor.
	LPDIRECTINPUTDEVICE8		m_mouseInput;			///<@todo Pewnie to powienien byæ wektor.

	std::vector< KeyboardState* >	m_keyboards;
	std::vector< MouseState* >		m_mouses;
	std::vector< JoystickState* >	m_joysticks;

public:
	explicit										DirectInputModule	();
	virtual											~DirectInputModule	();

	virtual bool									Init				( const InputInitInfo& initInfo ) override;

	virtual const std::vector< KeyboardState* >&	GetKeyboardStates	() override;
	virtual const std::vector< MouseState* >&		GetMouseStates		() override;
	virtual const std::vector< JoystickState* >&	GetJoystickStates	() override;

	virtual std::vector< const InputDeviceInfo* >	GetDevicesInfo		() override;

	virtual void									Update				( float timeInterval ) override;
	virtual bool									UpdateDevices		() override;

private:

	void			CleanDirectInput	();
	void			ClearInputStates	();

	void			UpdateKeyboard		( int idx );
	void			UpdateMouse			( int idx );
	void			UpdateJoystick		( int idx );
};

