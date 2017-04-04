#pragma once
/**
@file ThreadsBarrier.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"


#include <mutex>
#include <condition_variable>


namespace sw
{



/**@brief Can block multiple threads and wait for signal.
ThreadsBarrier is reusable.

@todo Implement*/
class ThreadsBarrier
{
private:

	std::mutex                  m_lock;
	std::condition_variable     m_condVariable;
	const uint16				m_numThreads;

protected:
public:
	explicit		ThreadsBarrier		( uint16 numThreads );
					~ThreadsBarrier		() = default;

};


}	// sw

