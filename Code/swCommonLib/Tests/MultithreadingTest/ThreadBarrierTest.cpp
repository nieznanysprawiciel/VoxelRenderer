#include "swCommonLib/External/Catch/catch.hpp"

#include "swCommonLib/Common/Multithreading/ThreadsBarrier.h"
#include "swCommonLib/Common/Multithreading/ThreadsLatch.h"

#include <vector>
#include <mutex>
#include <thread>


//====================================================================================//
//			ThreadsBarrier	
//====================================================================================//


const int numThreads = 4;

sw::ThreadsBarrier		gBarrier( numThreads );
std::mutex				gAccessCounterLock;
std::vector< int >		gAccessCounter;


// ================================ //
//
void	ThreadWaitingOnBarrier		()
{
	int numCycles = 0;

	while( numCycles < 5 )
	{
		gAccessCounterLock.lock();
		gAccessCounter.push_back( numCycles );
		gAccessCounterLock.unlock();

		numCycles++;

		gBarrier.ArriveAndWait();
	}
}




TEST_CASE( "ThreadBarrier - blocking" )
{
	std::thread threads[ numThreads ];

	for( auto& thread : threads )
		thread = std::thread( &ThreadWaitingOnBarrier );


	for( auto& thread : threads )
		thread.join();

	for( int i = 0; i < gAccessCounter.size(); i += numThreads )
	{
		for( int j = 0; j < numThreads - 1; j++ )
		{
			CHECK( gAccessCounter[ j ] == gAccessCounter[ j + 1 ] );
		}
	}
}


//====================================================================================//
//			ThreadsLatch	
//====================================================================================//

sw::ThreadsLatch		gLatch( numThreads );
std::vector< int >		gLatchAccessCounter;


// ================================ //
//
void	ThreadWaitingOnLatch		()
{
	int numCycles = 0;

	gAccessCounterLock.lock();
	gLatchAccessCounter.push_back( numCycles );
	gAccessCounterLock.unlock();

	numCycles++;

	gLatch.ArriveAndWait();

	gAccessCounterLock.lock();
	gLatchAccessCounter.push_back( numCycles );
	gAccessCounterLock.unlock();
}


TEST_CASE( "ThreadLatch - blocking" )
{
	std::thread threads[ numThreads ];

	for( auto& thread : threads )
		thread = std::thread( &ThreadWaitingOnLatch );


	for( auto& thread : threads )
		thread.join();

	for( int i = 0; i < gLatchAccessCounter.size(); i += numThreads )
	{
		for( int j = 0; j < numThreads - 1; j++ )
		{
			CHECK( gLatchAccessCounter[ j ] == gLatchAccessCounter[ j + 1 ] );
		}
	}
}


