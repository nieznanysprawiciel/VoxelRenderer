#include "FPSCounter.h"

#include <fstream>

#include "swCommonLib/System/Path.h"
#include "swCommonLib/System/Dir.h"


namespace vr
{

// ================================ //
//
FPSCounter::FPSCounter( uint32 refreshRate )
	:	m_lastRefreshTime( 0.0 )
	,	m_numFrames( 0 )
	,	m_refreshRate( refreshRate )
	,	m_avgMillisPerFrame( 0.0 )
	,	m_curSamplePtr( 0 )
{
	m_timeSamples.resize( 100, 0.0 );
}

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

		m_timeSamples[ m_curSamplePtr ] = m_avgMillisPerFrame;
		m_curSamplePtr = ( m_curSamplePtr + 1 ) % m_timeSamples.size();
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

// ================================ //
//
void			FPSCounter::PrintSamples		( const std::string & file )
{
	filesystem::Path perfFile( file );
	if( !perfFile.Exists() )
		filesystem::Dir::CreateDirectory( perfFile );

	std::fstream fileStream( file, std::fstream::out );
	if( fileStream.is_open() )
	{
		for( auto sample : m_timeSamples )
		{
			fileStream << sample << std::endl;
		}
	}
}

}	// vr
