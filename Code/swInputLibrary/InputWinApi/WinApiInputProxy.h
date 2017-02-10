#pragma once
/**
@file WinaApiInputProxy.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "swInputLibrary/IInput.h"
#include <Windows.h>


/**@brief Klasa do przekierowywania do silnika wejœcia pobranego z WPFa.*/
class WinApiInputProxy : public IInput
{
private:

	std::vector< KeyboardState* >	m_keyboards;	///< Uzywany jest tylko pierwszy element tablicy.
	std::vector< MouseState* >		m_mouses;		///< Uzywany jest tylko pierwszy element tablicy.
	std::vector< JoystickState* >	m_joysticks;	///< Uzywany jest tylko pierwszy element tablicy.

	double			m_lastX;
	double			m_lastY;

public:
	WinApiInputProxy();
	~WinApiInputProxy();

	virtual bool									Init				( const InputInitInfo& initInfo ) override;

	virtual const std::vector< KeyboardState* >&	GetKeyboardStates	() override;
	virtual const std::vector< MouseState* >&		GetMouseStates		() override;
	virtual const std::vector< JoystickState* >&	GetJoystickStates	() override;

	virtual std::vector< const InputDeviceInfo* >	GetDevicesInfo		() override;

	virtual void									Update				( float timeInterval ) override;
	virtual bool									UpdateDevices		() override;

public:
	///@name WinaAPI API
	///@{
	void				KeyboardChange				( int keyId, bool pressed );
	void				MouseButtonChange			( int button, bool pressed );
	void				MousePositionChange			( short X, short Y );
	void				MouseWheelChange			( double delta );

	void				LostFocus					();

	void				HandleEvent					( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	///@}
};

