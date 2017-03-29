#pragma once
/**
@file Demaphore.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/




#include "swCommonLib\Common\TypesDefinitions.h"

#include <mutex>
#include <condition_variable>


namespace sw
{



/**@brief Semaphore class.

@ingroup Multithreading.*/
class Semaphore
{
private:

	std::mutex                  m_lock;
	std::condition_variable     m_condVariable;
	Size						m_count;

public:

	explicit        Semaphore( Size initCount )
		: m_count( initCount )
	{}

	void    Down    ();
	bool    TryDown ();
	void    Up      ();

};



}	// sw

