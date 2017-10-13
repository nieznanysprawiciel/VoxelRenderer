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
	Size i = 0;
	for( ; i < m_keysTime.size(); i++ )
	{
		if( m_keysTime[ i ] < time )
			break;
	}

	if( i == m_keysTime.size() )
	{
		m_keysTime.push_back( time );
		m_keysValue.push_back( matrix );
	}
	else
	{
		m_keysTime.insert( m_keysTime.begin() + i, time );
		m_keysValue.insert( m_keysValue.begin() + i, matrix );
	}
}

}