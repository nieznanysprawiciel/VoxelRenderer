/**
@file Clock.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "Clock.h"


#include <chrono>
#include <math.h>
#include <assert.h>



namespace sw {
namespace gui
{

// ================================ //
//
Clock::Clock()
	:	m_ticksFromStart( 0 )
	,	m_startTime( 0 )
	,	m_elapsed( 0.0 )
	,	m_time( 0.0 )
	,	m_isFixedTimeStep( false )
	,	m_frameStep( 1.0f / 60.0f )
{}


// ================================ //
//
FrameTime					Clock::AdvanceFrame		()
{
	m_ticksFromStart = TimePoint();

	auto lastTime = m_time;

	m_time = ComputeTimeFromStart( m_ticksFromStart );
	m_elapsed = m_time - lastTime;

	if( m_isFixedTimeStep )
	{
		m_time = m_time - fmod( m_time, m_frameStep );
		m_elapsed = m_elapsed > m_time - lastTime ? m_frameStep : 0.0;
	}

	FrameTime frameTime;
	frameTime.Time = m_time;
	frameTime.Elapsed = m_elapsed;

	return frameTime;
}

// ================================ //
//
TimeRatio					Clock::GetFrameFraction() const
{
	TimeType frameTimeFromStart = ComputeTimeFromStart( m_ticksFromStart );
	TimeDiff lagFromFrameTime = frameTimeFromStart - m_time;

	return lagFromFrameTime / m_frameStep;
}

// ================================ //
//
void						Clock::SetFixedStep		( TimeDiff timeStep )
{
	assert( timeStep > 0.0f );

	m_isFixedTimeStep = true;
	m_frameStep = timeStep;
}

// ================================ //
//
void						Clock::StopFixedStep	()
{
	m_isFixedTimeStep = false;
}

// ================================ //
//
void						Clock::Start			()
{
	m_startTime = TimePoint();
	m_ticksFromStart = 0;
}

// ================================ //
//
Ticks						Clock::TimePoint		() const
{
	return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

// ================================ //
//
TimeType					Clock::ComputeTimeFromStart		( Ticks ticks ) const
{
	auto divider = std::chrono::high_resolution_clock::duration::period::num;
	return TimeType( ticks - m_startTime ) / divider;
}


}	// gui
}	// sw