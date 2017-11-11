#include "FPSCounter.h"



namespace vr
{

// ================================ //
//
FPSCounter::FPSCounter( uint32 refreshRate )
	:	m_lastRefreshTime( 0.0 )
	,	m_numFrames( 0 )
	,	m_refreshRate( refreshRate )
	,	m_avgMillisPerFrame( 0.0 )
{}

// ================================ //
//
void			FPSCounter::ProcessTime			( const sw::gui::FrameTime& frameTime )
{
	m_numFrames++;

	if( m_numFrames > m_refreshRate )
	{
		m_avgMillisPerFrame = 1000.0f * ( frameTime.Time - m_lastRefreshTime ) / m_numFrames;

		m_numFrames = 0;
		m_lastRefreshTime = frameTime.Time;
		m_needsRefresh = true;
	}
}

// ================================ //
//
bool			FPSCounter::NeedsFPSRefresh		() const
{
	return m_needsRefresh;
}

// ================================ //
//
std::string		FPSCounter::PrintFPS			()
{
	m_needsRefresh = false;

	return "Average frame time: [" + std::to_string( m_avgMillisPerFrame ) + " ms]";
}

}	// vr
