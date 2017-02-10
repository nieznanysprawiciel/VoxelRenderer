#pragma once

#include <assert.h>

/**@brief Stan przycisku.*/
class KeyState
{
	enum KeyStateFlag
	{
		Pressed = 0x1,
		DownEvent = 0x2,
		UpEvent = 0x4
	};

private:

	char	m_state;

public:

	inline bool		IsPressed		() const	{ return ( m_state & Pressed ) != 0; }
	inline bool		IsUp			() const	{ return !IsPressed(); }

	inline bool		IsKeyDownEvent	() const	{ return ( m_state & DownEvent ) != 0; }
	inline bool		IsKeyUpEvent	() const	{ return ( m_state & UpEvent ) != 0; }

	/// Umo¿liwia na umieszczanie w ifach if( KeyState )
	inline operator void*			() const	{ return (void*)IsPressed(); }

	inline void		operator=		( KeyState newState )
	{
		assert(!"");
	}

public:
	///@name Funkcje do ustawiania stanu (tylko dla dzieci IInput)
	///@{
	inline void		Press		()	{ m_state = Pressed | DownEvent; }	///< Wciska przycisk.
	inline void		UnPress		()	{ m_state = UpEvent; }				///< Puszcza przycisk.
	inline void		HoldState	()	{ m_state = m_state & Pressed; }	///< Podtrzymuje stan przycisku i kasuje info o eventach.

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


