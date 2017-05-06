#pragma once

#include "SpinLock.h"

#include <queue>
#include <mutex>	// std::lock_guard


/**@brief Kolejka z mechanizmami synchronizacji opartymi o SpinLocka.*/
template< typename ContentType >
class SpinLockedQueue
{
private:

	std::queue< ContentType >		m_queue;
	SpinLock						m_accesslock;

public:

	void		Push		( const ContentType& element );
	bool		TryPop		( ContentType& element );

	bool		IsEmpty		();

};

/**@brief */
template< typename ContentType >
inline void SpinLockedQueue< ContentType >::Push	( const ContentType& element )
{
	std::lock_guard< SpinLock > guard( m_accesslock );
	m_queue.push( element );
}

/**@brief Zwraca element w parametrze.

Je¿eli kolejka jest pusta element nie zostaje zmieniony, a funkcja zwraca false.*/
template< typename ContentType >
inline bool SpinLockedQueue< ContentType >::TryPop	( ContentType & element )
{
	std::lock_guard< SpinLock > guard( m_accesslock );

	if( m_queue.empty() )
		return false;

	element = m_queue.front();
	m_queue.pop();
	return true;
}

/**@brief Zwraca true, je¿eli kolejka jest pusta.
Nale¿y pamiêtaæ, ¿e po wywo³aniu tej funkcji kolejka mo¿e zostaæ opró¿niona
z innego w¹tku, wiêc nie nale¿y traktowaæ wyniku jak wyroczni.*/
template< typename ContentType >
inline bool SpinLockedQueue< ContentType >::IsEmpty	()
{
	std::lock_guard< SpinLock > guard( m_accesslock );
	return m_queue.empty();
}
