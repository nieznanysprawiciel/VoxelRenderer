#pragma once


#include <vector>
#include <DirectXMath.h>

#include "swCommonLib/Common/TypesDefinitions.h"


namespace vr
{

// ================================ //
//
struct Joint
{
	int						ParentIndex;
	uint64					ID;
	std::string				Name;
	DirectX::XMFLOAT4X4		GlobalBindposeInverse;
};



// ================================ //
//
class Skeleton
{
private:

	std::vector< Joint >		m_joints;

public:

	explicit		Skeleton	( std::vector< Joint > && joints );


	const std::vector< Joint >&		GetJoints	() const { return m_joints; }
};

DEFINE_PTR_TYPE( Skeleton );

}

