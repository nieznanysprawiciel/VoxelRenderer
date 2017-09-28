#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"

#include <DirectXMath.h>


namespace vr
{


const uint8	NUM_WEIGHTS = 4;

// ================================ //
//
struct ShellMeshVertex
{
	DirectX::XMFLOAT3			Position;
	uint8						WeightIdx[ NUM_WEIGHTS ];
	float						Weights[ NUM_WEIGHTS ];
};



}

