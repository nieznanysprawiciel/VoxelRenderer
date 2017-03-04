#pragma once


#include <DirectXMath.h>
#include <DirectXPackedVector.h>


namespace svo
{


/**@brief Attributes.
@todo Attributes should be better compressed in future.
@todo In first version only color is supported.*/
struct VoxelAttributes
{
	DirectX::PackedVector::XMCOLOR		Color;
};


}
