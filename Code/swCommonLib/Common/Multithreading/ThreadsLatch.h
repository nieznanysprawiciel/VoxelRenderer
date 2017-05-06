#pragma once
/**
@file ThreadsBarrier.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"


#include <mutex>
#include <atomic>
#include <condition_variable>


namespace sw
{



/**@brief Can block multiple threads and wait for signal.
ThreadsLatch is not reusable. Use ThreadsBarrier instead.

@ingroup Helpers*/
class ThreadsLatch
{
private:

	std::mutex                  m_lock;
	std::condition_variable     m_condVariable;
	uint16						m_remain;

protected:
public:
	explicit		ThreadsLatch		( uint16 numThreads );
					~ThreadsLatch		() = default;


	void			ArriveAndWait		();
};


}	// sw


