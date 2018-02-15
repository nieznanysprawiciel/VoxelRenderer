#pragma once


#include "swGUI/Core/System/Time/FrameTime.h"

#include <string>
#include <vector>


namespace vr
{

// ================================ //
//
class FPSCounter
{
private:

	sw::gui::TimeType		m_lastRefreshTime;
	uint32					m_numFrames;		///< Number of frames since last refresh.
	uint32					m_refreshRate;		///< Number of frames to refresh.

	bool					m_needsRefresh;		///< Check if you should call PrintFPS.
	sw::gui::TimeRatio		m_avgMillisPerFrame;

	std::vector< sw::gui::TimeRatio >		m_timeSamples;		///< Collects average time from multiple refreshes.
	Size									m_curSamplePtr;

public:

	explicit		FPSCounter	( uint32 refreshRate );

	void			ProcessTime		( const sw::gui::FrameTime& frameTime );
	std::string		PrintFPS		();
	void			PrintSamples	( const std::string& file );
	bool			NeedsFPSRefresh	() const;
};




}	// vr


