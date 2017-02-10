#pragma once
/**@file IInput.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

@brief Interfejs dla klas pobieraj¹cych dane o wejœciu u¿ytkownika.*/

#include "KeyboardState.h"
#include "MouseState.h"
#include "JoystickState.h"

#include "InputDeviceInfo.h"

#include <vector>

typedef void*	AppInstanceHandle;
typedef void*	WindowHandle;


/**@brief Zmienne parametryzuj¹ce inicjalizacjê modu³u wejœcia wyjœcia.
Na razie nie u¿ywane.*/
struct InputInitInfo
{
	AppInstanceHandle		AppInstance;
	WindowHandle			WndHandle;
};



/**@brief Interfejs dla klas pobieraj¹cych dane o wejœciu u¿ytkownika.

@todo Uzupe³niæ interfejs i wydzieliæ modu³y obs³uguj¹ce konkretne implementacje
urz¹dzeñ wejœciowych.*/
class IInput
{
private:
protected:
public:
	virtual				~IInput() = 0	{};

	/**@brief Inicjalizuje ca³y modu³. Po wywo³aniu tej funkcji modu³ musi byæ gotowy do u¿ytku
	w innym przypadku powinien zwróciæ false.*/
	virtual bool									Init				( const InputInitInfo& initInfo ) = 0;

	virtual const std::vector< KeyboardState* >&	GetKeyboardStates	() = 0;
	virtual const std::vector< MouseState* >&		GetMouseStates		() = 0;
	virtual const std::vector< JoystickState* >&	GetJoystickStates	() = 0;

	virtual std::vector< const InputDeviceInfo* >	GetDevicesInfo		() = 0;

	virtual void									Update				( float timeInterval ) = 0;

	/**@brief Funkcja na nowo sprawdza obecne w systemie urz¹dzenia i aktualizuje wszystkie
	potrzebne dane.*/
	virtual bool									UpdateDevices		() = 0;
};


