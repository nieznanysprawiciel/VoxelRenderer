#pragma once


#include "swGUI/Core/System/Time/FrameTime.h"

#include <string>


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

public:

	explicit		FPSCounter	( uint32 refreshRate );

	void			ProcessTime		( const sw::gui::FrameTime& frameTime );
	std::string		PrintFPS		();
	bool			NeedsFPSRefresh	() const;
};




}	// vr


