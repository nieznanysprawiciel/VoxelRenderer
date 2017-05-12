#pragma once
/**
@file KeyboardState.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "InputDeviceEvent.h"
#include "KeyState.h"

#include <vector>


namespace sw {
namespace input
{


const uint8 KEYBOARD_STATE_KEYS_NUMBER = 255;


/**@brief State of keyboard buttons.

@ingroup Input*/
class KeyboardState
{
private:

	KeyState				m_keyboardState[ KEYBOARD_STATE_KEYS_NUMBER ];

public:
	explicit		KeyboardState	();
					~KeyboardState	();

	/**@brief Gets array with keyboard state. Array has size KEYBOARD_STATE_KEYS_NUMBER.
	You can use Keyboard::PhysicalKeys to acces individual elements.*/
	const KeyState*				GetKeyboardState() const { return m_keyboardState; }

	/**@brief Access state of specific key.*/
	const KeyState&				operator[]		( Keyboard::PhysicalKeys key ) const		{ return m_keyboardState[ key ]; }

	///@name Funkcje do ustawiania stanu (tylko dla dzieci IInput)
	///@{
	KeyState*					KeysState		() { return m_keyboardState; }
	void						RemoveEvents	();
	///@}

	/**@brief Sets state depending on event.*/
	void						ApplyEvent		( const DeviceEvent& event );

public:

};



/**@brief */
inline			KeyboardState::KeyboardState()
{
	for( auto& val : m_keyboardState )
		val = 0;
}

/**@brief */
inline			KeyboardState::~KeyboardState()
{}

/**@brief Removes information about pressed or unpressed buttons.
Key states remain unchanged.*/
inline void		KeyboardState::RemoveEvents	()
{
	for( int i = 0; i < KEYBOARD_STATE_KEYS_NUMBER; ++i )
		m_keyboardState[ i ].HoldState();
}


// ================================ //
//
inline void		KeyboardState::ApplyEvent		( const DeviceEvent& event )
{
	if( event.Type == DeviceEventType::KeyboardEvent )
	{
		const KeyEvent& keyEvent = event.Key;
		KeyState& state = m_keyboardState[ keyEvent.Key ];
		state = keyEvent.State.IsPressed();
	}
}





}	// input
}	// sw

