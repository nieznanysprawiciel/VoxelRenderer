#pragma once


#include "swCommonLib/Common/TypesDefinitions.h"


#include <vector>
#include <DirectXMath.h>


namespace vr
{

typedef float TimeType;
typedef DirectX::XMFLOAT4X4 Transform;


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

private:

	std::vector< TimeType >::iterator		FindPlace		( TimeType time );
};



/**@brief */
class Animation
{
private:

	std::vector< JointAnimation >	m_jointsAnims;

protected:
public:
	explicit		Animation		( TemporaryAnimationInit& animInit );
	~Animation	() = default;

};

DEFINE_PTR_TYPE( Animation );



}

