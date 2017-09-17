#pragma once
/**
@file UniqueLock.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


namespace sw
{

/**@brief Equivalent of std::unique_lock class for ReaderWriterLock.*/
template< typename MutexType >
class WriterUniqueLock
{
private:

	MutexType&		m_lock;

public:

	WriterUniqueLock( MutexType& lock )
		: m_lock( lock )
	{
		Lock();
	}

	~WriterUniqueLock()
	{
		Unlock();
	}

	/**@brief Locks writer lock.*/
	void		Lock()
	{
		m_lock.WriterLock();
	}

	/**@brief Unlocks writer lock.*/
	void		Unlock()
	{
		m_lock.WriterUnlock();
	}

};

/**@brief Equivalent of std::unique_lock class for ReaderWriterLock.*/
template< typename MutexType >
class ReaderUniqueLock
{
private:

	MutexType&		m_lock;

public:

	ReaderUniqueLock( MutexType& lock )
		: m_lock( lock )
	{
		Lock();
	}

	~ReaderUniqueLock()
	{
		Unlock();
	}

	/**@brief Locks writer lock.*/
	void		Lock()
	{
		m_lock.ReaderLock();
	}

	/**@brief Unlocks writer lock.*/
	void		Unlock()
	{
		m_lock.ReaderUnlock();
	}

};



}	// sw

