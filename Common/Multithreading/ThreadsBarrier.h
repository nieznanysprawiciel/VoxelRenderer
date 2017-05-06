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

Implementation based on http://stackoverflow.com/questions/24465533/implementing-boostbarrier-in-c11

@todo Implement
@ingroup Helpers*/
class ThreadsBarrier
{
private:

	std::mutex                  m_lock;
	std::condition_variable     m_condVariable;
	Size						m_generation;
	const uint32				m_numThreads;
	uint32						m_remain;

protected:
public:
	explicit		ThreadsBarrier		( uint16 numThreads );
					~ThreadsBarrier		() = default;

	void			ArriveAndWait		();
};


}	// sw

