#pragma once
/**
@file QueueMT.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include <queue>
#include <mutex>
#include <condition_variable>


namespace sw
{



/**@brief Concurrent queue.*/
template< typename ContentType >
class QueueMT
{
private:

	std::queue< ContentType >		m_queue;
	std::mutex						m_accesslock;
	std::condition_variable			m_condition;

public:

	void		Push		( const ContentType& element );
	void		Push		( ContentType&& element );
	bool		TryPop		( ContentType& element );
	void		Pop			( ContentType& element );

	bool		IsEmpty		();

};

/**@brief */
template< typename ContentType >
inline void			QueueMT< ContentType >::Push	( const ContentType& element )
{
	std::lock_guard< std::mutex > guard( m_accesslock );
	m_queue.push( element );
}

/**@brief */
template< typename ContentType >
inline void			QueueMT< ContentType >::Push	( ContentType&& element )
{
	std::lock_guard< std::mutex > guard( m_accesslock );
	m_queue.push( std::move( element ) );
}


/**@brief Returns element in parameter.*/
template< typename ContentType >
inline void			QueueMT< ContentType >::Pop		( ContentType & element )
{
	std::unique_lock< std::mutex > guard( m_accesslock );

	while( m_queue.empty() )
	{
		m_condition.wait( guard );
	}

	element = std::move( m_queue.front() );
	m_queue.pop();
}

/**@brief */
template< typename ContentType >
inline bool			QueueMT< ContentType >::TryPop		( ContentType & element )
{
	std::unique_lock< std::mutex > guard( m_accesslock );

	if( m_queue.empty() )
	{
		return false;
	}

	element = std::move( m_queue.front() );
	m_queue.pop();
	return true;
}

/**@brief Zwraca true, je¿eli kolejka jest pusta.
Nale¿y pamiêtaæ, ¿e po wywo³aniu tej funkcji kolejka mo¿e zostaæ opró¿niona
z innego w¹tku, wiêc nie nale¿y traktowaæ wyniku jak wyroczni.*/
template< typename ContentType >
inline bool			QueueMT< ContentType >::IsEmpty	()
{
	std::lock_guard< std::mutex > guard( m_accesslock );
	return m_queue.empty();
}



}	// sw

