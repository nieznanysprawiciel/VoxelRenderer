#pragma once


#include "swCommonLib/Common/TypesDefinitions.h"

#include "ShellMeshTools/Animation/Skeleton.h"

#include <vector>
#include <DirectXMath.h>


namespace vr
{

typedef float TimeType;
typedef DirectX::XMFLOAT4X4 Transform;
typedef DirectX::XMMATRIX Matrix;


struct TemporaryAnimationInit;


// ================================ //
//
class JointAnimation
{
private:

	std::vector< TimeType >		m_keysTime;
	std::vector< Transform >	m_keysValue;

public:
	explicit	JointAnimation	() = default;

	void		AddKey		( TimeType time, const Transform& matrix );
	Matrix 		Evaluate	( TimeType time );

private:

	std::vector< TimeType >::iterator		FindPlace		( TimeType time );

	Matrix		LinearInterpolate	( TimeType time, TimeType key1Time, TimeType key2Time, const Transform& transform1, const Transform& transform2 );
};



// ================================ //
//
class Animation
{
private:

	std::vector< JointAnimation >	m_jointsAnims;
	TimeType						m_animLength;
	SkeletonPtr						m_skeleton;

protected:
public:
	explicit		Animation		( SkeletonPtr skeleton, TemporaryAnimationInit& animInit );
	~Animation	() = default;

	std::vector< Transform >		Evaluate		( TimeType time );

};

DEFINE_PTR_TYPE( Animation );



}

