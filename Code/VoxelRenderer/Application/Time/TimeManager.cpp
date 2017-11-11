/**
@file TimeManager.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "TimeManager.h"


using namespace sw::input;


namespace vr
{

// ================================ //
//
TimeManager::TimeManager()
	:	m_fpsCounter( 60 )
	,	m_isPaused( false )
	,	m_pauseTime( 0.0f )
{}

// ================================ //
//
void		TimeManager::ProcessTime		( const sw::gui::FrameTime & frameTime )
{
	m_fpsCounter.ProcessTime( frameTime );

	m_frameTime.Elapsed = frameTime.Elapsed;

	if( m_isPaused )
	{
		m_pauseTime += frameTime.Elapsed;
		m_frameTime.Elapsed = 0.0;
	}

	m_frameTime.Time = frameTime.Time - m_pauseTime;
}

// ================================ //
//
void		TimeManager::ProcessInput		( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )
{
	// Toggle pause state.
	if( keyboard[ Keyboard::PhysicalKeys::KEY_PAUSE ].IsKeyUpEvent() )
	{
		m_isPaused = !m_isPaused;
	}
}

}	// vr

