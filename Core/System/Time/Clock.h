#pragma once
/**
@file Clock.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "FrameTime.h"



namespace sw {
namespace gui
{


/**@brief Provides time for application.

Clock doesn't queries time every call. Instead it computes time value at the begining of each frame.
In most use case this is the time that we are interested in.

Call start to init timer and then call AdvanceFrame when you want to query and recompute time values.*/
class Clock
{
private:

	Ticks			m_startTime;
	Ticks			m_ticksFromStart;

	TimeType		m_time;				///< Current time.
	TimeDiff		m_elapsed;			///< Elapsed since last frame.

	bool			m_isFixedTimeStep;	///< m_time value can be only multiply of m_frameStep.

	TimeDiff		m_frameStep;

protected:
public:
	explicit		Clock	();
					~Clock	() = default;

	/**@brief Call on the beginning of each frame.*/
	FrameTime			AdvanceFrame	();

	/**@brief Returns time from clock initialization.*/
	TimeType			TimeFromStart	() const { return m_time; }

	/**@brief Returns time that elapsed since last frame.*/
	TimeDiff			Elapsed			() const { return m_elapsed; }

	/**@brief If fixed time step is set, this function returns fraction of next frame that elapsed. If fixed step is not set,
	this function returns always 0.0.*/
	TimeRatio			GetFrameFraction() const;

	/**@brief Changes mode to fixed time step.*/
	void				SetFixedStep	( TimeDiff timeStep );

	/**@brief Change mode to real time mode.*/
	void				StopFixedStep	();

public:

	void				Start			();


private:

	Ticks				TimePoint				() const;
	TimeType			ComputeTimeFromStart	( Ticks ticks ) const;
};




}	// gui
}	// sw



