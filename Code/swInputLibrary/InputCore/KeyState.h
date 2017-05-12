#pragma once
/**
@file KeyState.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "swCommonLib/Common/TypesDefinitions.h"

#include <assert.h>




namespace sw {
namespace input
{



/**@brief Button state structure..

@ingroup Input*/
class KeyState
{
	enum KeyStateFlag : int8
	{
		Pressed = 0x1,
		DownEvent = 0x2,
		UpEvent = 0x4
	};

private:

	int8			m_state;

public:

	explicit		KeyState()
		: m_state( 0 )
	{}

	inline bool		IsPressed		() const { return ( m_state & Pressed ) != 0; }
	inline bool		IsUp			() const { return !IsPressed(); }

	inline bool		IsKeyDownEvent	() const { return ( m_state & DownEvent ) != 0; }
	inline bool		IsKeyUpEvent	() const { return ( m_state & UpEvent ) != 0; }

	/// Umo¿liwia na umieszczanie w ifach if( KeyState )
	inline operator void*			( ) const { return (void*)IsPressed(); }

	inline void		operator=		( KeyState newState )
	{
		m_state = newState.m_state;
	}

public:
	///@name Funkcje do ustawiania stanu (tylko dla dzieci IInput)
	///@{
	inline void		Press		() { m_state = Pressed | DownEvent; }	///< Wciska przycisk.
	inline void		UnPress		() { m_state = UpEvent; }				///< Puszcza przycisk.
	inline void		HoldState	() { m_state = m_state & Pressed; }	///< Podtrzymuje stan przycisku i kasuje info o eventach.

	inline void		operator=	( bool newState )
	{
		if( !IsPressed() && newState )
			Press();
		else if( IsPressed() && !newState )
			UnPress();
		else
			HoldState();
	}
	///@}
};


}	// input
}	// sw

