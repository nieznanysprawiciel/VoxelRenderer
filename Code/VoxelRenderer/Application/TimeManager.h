#pragma once


#include "swCommonLib/Common/TypesDefinitions.h"

#include "swInputLibrary/InputCore/KeyboardState.h"
#include "swInputLibrary/InputCore/MouseState.h"

#include "swGUI/Core/System/Time/FrameTime.h"



namespace vr
{

// ================================ //
//
class TimeManager
{
private:

	bool					m_isPaused;
	sw::gui::TimeType		m_pauseTime;

	sw::gui::FrameTime		m_frameTime;

public:
	TimeManager();
	~TimeManager() = default;

	void			ProcessTime			( const sw::gui::FrameTime& frameTime );
	void			ProcessInput		( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard );

public:

	sw::gui::FrameTime		GetCurrentFrameTime	() { return m_frameTime; }
};


}

