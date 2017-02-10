/**
@file RelativeTimeline.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "RelativeTimeline.h"


// ================================ //
//
RelativeTimeline::RelativeTimeline()
	:	TimelineBase( nullptr )
	,	m_offset( 0.0 )
	,	m_factor( 1.0f )
	,	m_duration( std::numeric_limits< TimeType >::max() )
	,	m_preWrap( WrapMode::Clamp )
	,	m_postWrap( WrapMode::Clamp )
	,	m_isPaused( false )
	,	m_isStarted( false )
{}

// ================================ //
//
RelativeTimeline::RelativeTimeline( TimelineBasePtr parent )
	:	TimelineBase( parent )
	,	m_offset( 0.0 )
	,	m_factor( 1.0f )
	,	m_duration( std::numeric_limits< TimeType >::max() )
	,	m_preWrap( WrapMode::Clamp )
	,	m_postWrap( WrapMode::Clamp )
	,	m_isPaused( false )
	,	m_isStarted( false )
{}

// ================================ //
//
void		RelativeTimeline::SetPreWrap	( WrapMode mode )
{	m_preWrap = mode;	}

// ================================ //
//
void		RelativeTimeline::SetPostWrap	( WrapMode mode )
{	m_postWrap = mode;	}

// ================================ //
//
void		RelativeTimeline::SetDuration	( TimeType duration )
{	m_duration = duration;	}

// ================================ //
//
void		RelativeTimeline::Update()
{
	if( auto parent = m_parent.lock() )
	{
		if( m_isStarted && !m_isPaused )
		{
			m_currentTime = EvalTime( parent->GetTime() );
		}
		else if( m_isPaused )
		{
			m_offset += parent->GetTime() - m_currentTime;
		}
	}
	else
	{
		m_currentTime = TimeType( 0.0 );
	}
}

// ================================ //
//
void		RelativeTimeline::Start()
{
	if( m_isPaused )
	{
		m_isPaused = false;
	}
	else if( auto parent = m_parent.lock() )
	{
		m_offset = parent->GetTime();
		m_currentTime = EvalTime( parent->GetTime() );
		m_isStarted = true;
	}
}

// ================================ //
//
void		RelativeTimeline::Stop()
{
	m_isStarted = false;
	m_isPaused = false;
	m_currentTime = TimeType( 0.0 );
}

// ================================ //
//
void		RelativeTimeline::Pause()
{	m_isPaused = true;	}

// ================================ //
//
void		RelativeTimeline::SetTimeline( TimelineBasePtr& parent )
{	m_parent = parent;	}

// ================================ //
//
bool		RelativeTimeline::IsPaused()
{	return m_isPaused;	}


//====================================================================================//
//			Internal	
//====================================================================================//

// ================================ //
//
TimeType	RelativeTimeline::EvalElapsedTime	( TimeType parentTime )
{
	TimeType elapsed = parentTime - m_offset;
	elapsed *= m_factor;

	return elapsed;
}

// ================================ //
//
TimeType	RelativeTimeline::EvalTime			( TimeType parentTime )
{
	TimeType elapsed = EvalElapsedTime( parentTime );

	while( elapsed > m_duration || elapsed < 0.0 )
	{
		if( elapsed > m_duration )
		{
			switch( m_postWrap )
			{
				case WrapMode::Clamp:
					return m_duration;
				case WrapMode::Repeat:
				{
					int repeats = static_cast< int >( elapsed / m_duration );
					elapsed = elapsed - repeats * m_duration;
					break;
				}
				case WrapMode::Mirror:
					elapsed = 2 * m_duration - elapsed;
					break;
			}
		}
		else
		{
			switch( m_preWrap )
			{
				case WrapMode::Clamp:
					return TimeType( 0.0 );
				case WrapMode::Repeat:
					elapsed = m_duration + elapsed;
					break;
				case WrapMode::Mirror:
					elapsed = -elapsed;
					break;
			}
		}
	}

	return elapsed;
}


