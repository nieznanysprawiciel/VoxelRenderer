#pragma once
/**
@file MouseState.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "KeyState.h"
#include "InputDeviceEvent.h"


namespace sw {
namespace input
{

/**@brief Max number of mouse buttons.*/
const int8	MOUSE_STATE_MAX_NUM_BUTTONS = 8;
/**@brief Max number of mouse axes.*/
const int8	MOUSE_STATE_MAX_NUM_AXES = 4;



/**@brief State of mouse.

@ingroup Input*/
class MouseState
{
private:

	float				m_axes[ 4 ];
	uint16				m_position[ 2 ];	///< Wspó³rzêdne X i Y.
	KeyState			m_buttons[ MOUSE_STATE_MAX_NUM_BUTTONS ];

public:
	MouseState();
	~MouseState();

	const float*				GetAxesState	() const { return m_axes; }
	const KeyState*				GetButtonsState	() const { return m_buttons; }

	uint16						GetPositionX	() const { return m_position[ 0 ]; }
	uint16						GetPositionY	() const { return m_position[ 1 ]; }

	void						SetPosition		( short X, short Y );

	float						WheelDelta		() const { m_axes[ Mouse::PhysicalAxes::WHEEL ]; }

	const KeyState*				LeftButton		() const;
	const KeyState*				RightButton		() const;
	const KeyState*				MiddleButton	() const;
	const KeyState*				XButton1		() const;
	const KeyState*				XButton2		() const;

	const KeyState&				operator[]		( Mouse::PhysicalButtons button ) const { return m_buttons[ button ]; }

	///@name Functions for setting state (only for IInput)
	///@{
	void						RemoveEvents	();
	///@}


	/**@brief Updates state depending on event.*/
	void						ApplyEvent		( const ButtonEvent& event );

	/**@brief Updates state depending on event.*/
	void						ApplyEvent		( const AxisEvent& event );

	/**@brief Updates state depending on event.*/
	void						ApplyEvent		( const CursorEvent& event );

	/**@brief Updates state depending on event.*/
	void						ApplyEvent		( const DeviceEvent& event );
};

/**@brief */
inline MouseState::MouseState()
{
	for( auto& val : m_axes )
		val = 0.0f;
	for( auto& val : m_buttons )
		val = 0;

	m_position[ 0 ] = 0;
	m_position[ 1 ] = 0;
}

/**@brief */
inline MouseState::~MouseState()
{}

/**@brief Ustawia pozycjê myszy.

Powinno byæ u¿ywane jedynie przez modu³ wejœcia (@ref IInput).*/
inline void			MouseState::SetPosition			( short X, short Y )
{
	m_position[ 0 ] = X;
	m_position[ 1 ] = Y;
}

/**@brief Czyœci tablicê z eventów o wciœniêciu klawiszy, ale podtrzymuje stan przycisków.*/
inline void			MouseState::RemoveEvents	()
{
	for( int i = 0; i < MOUSE_STATE_MAX_NUM_BUTTONS; ++i )
		m_buttons[ i ].HoldState();
}

// ================================ //
//
inline void			MouseState::ApplyEvent		( const ButtonEvent& event )
{
	KeyState& state = m_buttons[ event.Button ];
	state = event.State.IsPressed();
}

// ================================ //
//
inline void			MouseState::ApplyEvent		( const AxisEvent& event )
{
	float& axis = m_axes[ event.Axis ];
	axis += event.Delta;
}

// ================================ //
//
inline void			MouseState::ApplyEvent		( const CursorEvent& event )
{
	m_position[ 0 ] += event.OffsetX;
	m_position[ 1 ] += event.OffsetY;
}

// ================================ //
//
inline void			MouseState::ApplyEvent		( const DeviceEvent& event )
{
	switch( event.Type )
	{
		case DeviceEventType::ButtonEvent:
			ApplyEvent( event.Button );
			break;
		case DeviceEventType::AxisEvent:
			ApplyEvent( event.Axis );
			break;
		case DeviceEventType::CursorEvent:
			ApplyEvent( event.Cursor );
			break;
		default:
			break;
	}
}


//====================================================================================//
//			Helper accessor	
//====================================================================================//

// ================================ //
//
inline const KeyState*		MouseState::LeftButton() const
{
	return &m_buttons[ Mouse::LEFT_BUTTON ];
}

// ================================ //
//
inline const KeyState*		MouseState::RightButton() const
{
	return &m_buttons[ Mouse::RIGHT_BUTTON ];
}

// ================================ //
//
inline const KeyState*		MouseState::MiddleButton() const
{
	return &m_buttons[ Mouse::MIDDLE_BUTTON ];
}

// ================================ //
//
inline const KeyState*		MouseState::XButton1() const
{
	return &m_buttons[ Mouse::PhysicalButtons::XBUTTON1 ];
}

// ================================ //
//
inline const KeyState*		MouseState::XButton2() const
{
	return &m_buttons[ Mouse::PhysicalButtons::XBUTTON2 ];
}


}	// input
}	// sw
