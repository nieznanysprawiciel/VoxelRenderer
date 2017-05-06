/**
@file ThreadsBarrier.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "ThreadsBarrier.h"




namespace sw
{


// ================================ //
//
ThreadsBarrier::ThreadsBarrier( uint16 numThreads )
	:	m_numThreads( numThreads )
	,	m_remain( numThreads )
	,	m_generation( 0 )
{}

// ================================ //
//
void		ThreadsBarrier::ArriveAndWait()
{
	std::unique_lock< std::mutex > lock( m_lock );
	
	auto curGeneration = m_generation;
	m_remain--;
	
	if( m_remain == 0 )
	{
		m_generation++;
		m_remain = m_numThreads;
		m_condVariable.notify_all();
	}
	else
	{
		while( curGeneration != m_generation )
			m_condVariable.wait( lock );
	}
}





}	// sw

