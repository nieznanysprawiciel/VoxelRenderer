#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"

#include <DirectXMath.h>


namespace vr
{

#define MAX_LIGHTS 2


/**@brief Description of light parameters.*/
struct PointLightParams
{
	DirectX::XMFLOAT3		Color;
	float					ClampRadius;
	DirectX::XMFLOAT3		Position;
	float					ConstAttenuation;
	float					LinearAttenuation;
	float					QuadraticAttenuation;
	float					Intensity;

	// ================================ //
	//
	PointLightParams()
		:	Position( DirectX::XMFLOAT3( 0.0, 0.0, 0.0 ) )
		,	Color( DirectX::XMFLOAT3( 1.0, 1.0, 1.0 ) )
		,	ClampRadius( 300000.0f )
		,	ConstAttenuation( 0.0f )
		,	LinearAttenuation( 0.0f )
		,	QuadraticAttenuation( 1.0f )
		,	Intensity( 1.0f )
	{}
};

/**@brief Description of light parameters.*/
struct DirectionalLightParams
{
	DirectX::XMFLOAT3		Color;
	float					Intensity;
	DirectX::XMFLOAT3		Direction;
	float					Align2;

	// ================================ //
	//
	DirectionalLightParams()
		:	Direction( DirectX::XMFLOAT3( 0.0, 0.0, -1.0 ) )
		,	Color( DirectX::XMFLOAT3( 1.0, 1.0, 1.0 ) )
		,	Intensity( 1.0 )
	{}
};

/**@brief Constant buffer layout with light data.*/
struct LightConstants
{
	DirectionalLightParams	DirectionalLight;
	PointLightParams		LightData[ MAX_LIGHTS ];
};



}
