#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"

#include <DirectXMath.h>


namespace vr
{

#define MAX_LIGHTS 3



/**@brief Type of light.
@ingroup Lights*/
enum class LightType : int32
{
	PointLight = 0,
	SpotLight,
	DirectionalLight
};



/**@brief Description of light parameters.
@ingroup Lights*/
struct LightParams
{
	DirectX::XMFLOAT3		Color;
	float					ClampRadius;
	DirectX::XMFLOAT3		Position;
	float					SpotAngle;
	DirectX::XMFLOAT3		Direction;		// Wspó³rzêdne s¹ zanegowane, ¿eby shader mia³ mniej roboty
	LightType				Type;
	float					ConstAttenuation;
	float					LinearAttenuation;
	float					QuadraticAttenuation;
	float					Intensity;

	// ================================ //
	//
	LightParams()
		:	Position( DirectX::XMFLOAT3( 0.0, 0.0, 0.0 ) )
		,	Color( DirectX::XMFLOAT3( 1.0, 1.0, 1.0 ) )
		,	Direction( DirectX::XMFLOAT3( 0.0, 0.0, 1.0 ) )
		,	ClampRadius( 300000.0f )
		,	SpotAngle( 30.0f )
		,	Type( LightType::PointLight )
		,	ConstAttenuation( 0.0f )
		,	LinearAttenuation( 0.0f )
		,	QuadraticAttenuation( 1.0f )
		,	Intensity( 1.0f )
	{}
};


/**@brief Constant buffer layout with light data.

@see LightParams
@ingroup ConstantBuffers
@ingroup Lights*/
struct LightConstants
{
	DirectX::XMFLOAT3		AmbientColor;
	uint32					NumLights;			///< Number of lights. Must be lower then ENGINE_MAX_LIGHTS.
	LightParams				LightData[ MAX_LIGHTS ];
};


}
