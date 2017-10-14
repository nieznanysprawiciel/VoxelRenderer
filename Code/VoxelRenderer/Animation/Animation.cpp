#include "Animation.h"

#include "VoxelRenderer/ShellMesh/FbxHelperStructs.h"


namespace vr
{

// ================================ //
//
Animation::Animation		( TemporaryAnimationInit& animInit )
	:	m_jointsAnims( animInit.JointsAnims )
{}



//====================================================================================//
//			JointAnimation	
//====================================================================================//


// ================================ //
//
void			JointAnimation::AddKey		( TimeType time, const Transform& matrix )
{
	auto timeIter = FindPlace( time );

	if( timeIter == m_keysTime.end() )
	{
		m_keysTime.push_back( time );
		m_keysValue.push_back( matrix );

		return;
	}

	auto distance = std::distance( m_keysTime.begin(), timeIter );
	auto valueIter = m_keysValue.begin() + distance;

	// Key exists. Update it.
	if( abs( *timeIter - time ) < std::numeric_limits< float >::epsilon() )
	{
		*valueIter = matrix;
		return;
	}

	// Add new key in the middle of vector.
	m_keysTime.insert( timeIter, time );
	m_keysValue.insert( valueIter, matrix );
}

// ================================ //
//
std::vector< TimeType >::iterator		JointAnimation::FindPlace		( TimeType time )
{
	return std::lower_bound( m_keysTime.begin(), m_keysTime.end(), time );
}

}