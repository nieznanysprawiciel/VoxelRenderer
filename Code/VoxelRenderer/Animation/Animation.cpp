#include "Animation.h"

#include "VoxelRenderer/ShellMesh/FbxHelperStructs.h"


namespace vr
{

// ================================ //
//
Animation::Animation		( TemporaryAnimationInit& animInit )
	:	m_jointsAnims( animInit.JointsAnims )
	,	m_animLength( 0.0 )
{
	auto length = animInit.End - animInit.Start;
	m_animLength = (TimeType)length.GetSecondDouble();
}

// ================================ //
//
std::vector< Transform >		Animation::Evaluate		( TimeType time )
{
	std::vector< Transform > animMats;
	animMats.resize( m_jointsAnims.size() );

	auto wrappedTime = fmod( time, m_animLength );

	for( int i = 0; i < m_jointsAnims.size(); ++i )
	{
		DirectX::XMMATRIX frameTransform = m_jointsAnims[ i ].Evaluate( wrappedTime );
		frameTransform = DirectX::XMMatrixTranspose( frameTransform );

		DirectX::XMStoreFloat4x4( &animMats[ i ], frameTransform );
	}

	return animMats;
}



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
Matrix									JointAnimation::Evaluate		( TimeType time )
{
	if( m_keysTime.size() == 0 )
		return DirectX::XMMatrixIdentity();

	if( m_keysTime.size() == 1 )
		return DirectX::XMLoadFloat4x4( &m_keysValue[ 0 ] );

	auto iter = m_keysTime.begin();
	for( ; iter != m_keysTime.end(); ++iter )
	{
		if( *iter > time )
			break;
	}

	Size keyIdx = std::distance( m_keysTime.begin(), iter );

	if( keyIdx >= m_keysTime.size() )
		return DirectX::XMLoadFloat4x4( &m_keysValue.back() );
	if( keyIdx == 0 )
		return DirectX::XMLoadFloat4x4( &m_keysValue[ 0 ] );

	return LinearInterpolate( time, m_keysTime[ keyIdx - 1 ], m_keysTime[ keyIdx ], m_keysValue[ keyIdx - 1 ], m_keysValue[ keyIdx ] );
}

// ================================ //
//
std::vector< TimeType >::iterator		JointAnimation::FindPlace		( TimeType time )
{
	return std::lower_bound( m_keysTime.begin(), m_keysTime.end(), time );
}

// ================================ //
//
Matrix					JointAnimation::LinearInterpolate		( TimeType time, 
																  TimeType key1Time, 
																  TimeType key2Time, 
																  const Transform & transform1, 
																  const Transform & transform2 )
{
	TimeType timeInterval = key2Time - key1Time;
	TimeType progress = ( time - key1Time ) / timeInterval;

	DirectX::XMMATRIX leftMatrix = DirectX::XMLoadFloat4x4( &transform1 );
	DirectX::XMMATRIX rightMatrix = DirectX::XMLoadFloat4x4( &transform2 );

	auto w1 = ( 1.0 + (-1.0f) * progress );	// Multiply by -1.0f to avoid calling operator-
	auto w2 = progress;

	return leftMatrix * w1 + rightMatrix * w2;
}

}	// vr

