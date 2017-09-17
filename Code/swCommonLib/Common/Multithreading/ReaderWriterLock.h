#pragma once
/**
@file ReaderWriterLock.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include <atomic>
#include <mutex>

namespace sw
{



/**@brief Readers-Writers problem solution with readers preference.*/
class ReaderWriterLock
{
private:

	std::mutex		m_resourceLock;
	std::mutex		m_readersLock;
	int				m_numReaders;

protected:
public:
	explicit		ReaderWriterLock		() = default;
	~ReaderWriterLock	() = default;


	inline void			ReaderLock		();
	inline void			ReaderUnlock	();
	inline void			WriterLock		();
	inline void			WriterUnlock	();
};


//====================================================================================//
//			Implementation	
//====================================================================================//


// ================================ //
//
inline void			ReaderWriterLock::ReaderLock		()
{
	std::lock_guard< std::mutex > guard( m_readersLock );

	int prevVal = m_numReaders++;
	if( prevVal == 0 )
		m_resourceLock.lock();
}

// ================================ //
//
inline void			ReaderWriterLock::ReaderUnlock		()
{
	std::lock_guard< std::mutex > guard( m_readersLock );

	int curVal = --m_numReaders;
	if( curVal == 0 )
		m_resourceLock.unlock();
}

// ================================ //
//
inline void			ReaderWriterLock::WriterLock		()
{
	m_resourceLock.lock();
}

// ================================ //
//
inline void			ReaderWriterLock::WriterUnlock		()
{
	m_resourceLock.unlock();
}

}	// sw



