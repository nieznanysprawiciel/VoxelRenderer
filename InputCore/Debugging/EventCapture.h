#pragma once
/**
@file EventCapture.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swInputLibrary/InputCore/InputDeviceEvent.h"
#include "swInputLibrary/InputCore/EventQueue.h"

#include <vector>


namespace sw {
namespace input
{

/**@brief Event structure for debugging.
This structure holds additionally FrameNumber. This allows user to provide full set of input events
ordered by sending time.

@ingroup DebugInput*/
struct DebugEvent
{
	DeviceEvent			EventContent;
	Size				FrameNumber;
	uint8				DeviceIdx;
	DeviceType			DeviceType;
};



/**@brief This class provides functionality of capturing events for debug and test purposes.

@ingroup DebugInput*/
class EventCapture
{
private:

	EventQueue< DebugEvent >		m_events;
	
	Size			m_frameNumber;
	Timestamp		m_nextTimeStamp;

protected:
public:
	explicit		EventCapture		();
					~EventCapture		() = default;

	///@name Manually add events.
	///@{

	/**@brief Adds key event to current frame and gives it next timstamp.*/
	void			QueueUpKeyEvent		( Keyboard::PhysicalKeys key );
	void			QueueDownKeyEvent	( Keyboard::PhysicalKeys key );
	void			QueueKeyEvent		( KeyState state, Keyboard::PhysicalKeys key );

	void			QueueMouseClick		( Mouse::PhysicalButtons button );
	void			QueueMouseUp		( Mouse::PhysicalButtons button );
	void			QueueMouseEvent		( KeyState state, Mouse::PhysicalButtons key );

	void			QueueMouseMove		( int16 deltaX, int16 deltaY );
	///@}

public:

	///@name For DebugInput
	///@{
	void					GoToNextFrame		();

	const DebugEvent&		QueryEvent			( Size frameNum );
	bool					IsNext				( Size frameNum );

	void			QueueEvent			( const DeviceEvent& event, Size frameNum, DeviceType deviceType, uint8 deviceIdx = 0 );
	void			QueueEvent			( const DebugEvent& event );
	///@}

};


}	// input
}	// sw


