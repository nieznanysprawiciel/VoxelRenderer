/**
@file ThreadsLatch.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "ThreadsLatch.h"




namespace sw
{


// ================================ //
//
ThreadsLatch::ThreadsLatch( uint16 numThreads )
	:	m_remain( numThreads )
{}

// ================================ //
//
void		ThreadsLatch::ArriveAndWait()
{
	std::unique_lock< std::mutex > lock( m_lock );
	
	if( m_remain == 0 )
	{
		m_condVariable.notify_all();
	}
	else
	{
		while( m_remain != 0 )
			m_condVariable.wait( lock );
	}
}





}	// sw

