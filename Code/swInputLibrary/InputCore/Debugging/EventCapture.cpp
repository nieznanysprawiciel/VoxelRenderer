#include "EventCapture.h"
#include "EventCapture.h"
#include "EventCapture.h"
#include "EventCapture.h"
#include "EventCapture.h"
/**
@file EventCapture.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "EventCapture.h"


namespace sw {
namespace input
{




// ================================ //
//
EventCapture::EventCapture()
	:	m_nextTimeStamp( 0 )
	,	m_frameNumber( 0 )
{}

// ================================ //
//
void			EventCapture::QueueUpKeyEvent	( Keyboard::PhysicalKeys key )
{
	KeyState state;
	state.UnPress();

	QueueKeyEvent( state, key );
}

// ================================ //
//
void			EventCapture::QueueDownKeyEvent	( Keyboard::PhysicalKeys key )
{
	KeyState state;
	state.Press();

	QueueKeyEvent( state, key );
}

// ================================ //
//
void			EventCapture::QueueKeyEvent		( KeyState state, Keyboard::PhysicalKeys key )
{
	KeyEvent keyEvent;
	keyEvent.State = state;
	keyEvent.Key = key;

	DeviceEvent newEvent( keyEvent, m_nextTimeStamp++ );
	QueueEvent( newEvent, m_frameNumber, DeviceType::Keyboard, 0 );
}

// ================================ //
//
void			EventCapture::QueueMouseClick	( Mouse::PhysicalButtons button )
{
	KeyState state;
	state.Press();

	QueueMouseEvent( state, button );
}

// ================================ //
//
void			EventCapture::QueueMouseUp		( Mouse::PhysicalButtons button )
{
	KeyState state;
	state.UnPress();

	QueueMouseEvent( state, button );
}

// ================================ //
//
void			EventCapture::QueueMouseEvent	( KeyState state, Mouse::PhysicalButtons key )
{
	ButtonEvent buttonEvent;
	buttonEvent.State = state;
	buttonEvent.Button = key;

	DeviceEvent newEvent( buttonEvent, m_nextTimeStamp++ );
	QueueEvent( newEvent, m_frameNumber, DeviceType::Mouse, 0 );
}

// ================================ //
//
void			EventCapture::QueueMouseMove	( int16 deltaX, int16 deltaY )
{
	CursorEvent cursorMoved;
	cursorMoved.OffsetX = deltaX;
	cursorMoved.OffsetY = deltaY;

	DeviceEvent newEvent( cursorMoved, m_nextTimeStamp++ );
	QueueEvent( newEvent, m_frameNumber, DeviceType::Mouse, 0 );
}

// ================================ //
//
void			EventCapture::QueueEvent		( const DeviceEvent& event, Size frameNum, DeviceType deviceType, uint8 deviceIdx )
{
	DebugEvent evt;
	memcpy( &evt.EventContent, &event, sizeof( event ) );
	evt.FrameNumber = frameNum;
	evt.DeviceType = deviceType;
	evt.DeviceIdx = deviceIdx;

	QueueEvent( evt );
}

// ================================ //
/// @todo Sort events.
void			EventCapture::QueueEvent		( const DebugEvent& event )
{
	m_events.AddEvent( event );
}

// ================================ //
/// @todo Don't ignore frameNum
const DebugEvent&	EventCapture::QueryEvent	( Size frameNum )
{
	if( !m_events.NoMoreEvents() )
	{
		return m_events.PopEvent();
	}

	DebugEvent evt;
	return evt;
}

// ================================ //
//
bool				EventCapture::IsNext		( Size frameNum )
{
	if( m_events.NoMoreEvents() )
		return false;

	return m_events.FrontEvent().FrameNumber == frameNum;
}

// ================================ //
//
void				EventCapture::GoToNextFrame	()
{
	m_nextTimeStamp = 0;
	m_frameNumber++;
}

}	// input
}	// sw
