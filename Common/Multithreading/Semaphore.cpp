/**
@file Sempahore.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "Semaphore.h"



namespace sw
{


// ================================ //
//
void			Semaphore::Down		()
{
	std::unique_lock< std::mutex > lock( m_lock );
	while( m_count <= 0 )
		m_condVariable.wait( lock );

	m_count--;
}

// ================================ //
//
bool			Semaphore::TryDown	()
{
	std::unique_lock< std::mutex > lock( m_lock );

	if( m_count > 0 )
	{
		m_count--;
		return true;
	}

	return false;
}

// ================================ //
//
void			Semaphore::Up		()
{
	std::unique_lock< std::mutex > lock( m_lock );
	m_count++;

	m_condVariable.notify_one();
}

}	// sw


