

#define MAX_LIGHTS 3

#define POINT_LIGHT 0
#define SPOT_LIGHT 1
#define DIRECTIONAL_LIGHT 2



#define SPECULAR_POWER 100


// ================================ //
//
struct LightParams
{
	float3			Color;
	float			ClampRadius;
	float3			Position;
	float			SpotAngle;
	float3			Direction;
	uint			LightType;
	float			ConstantAttenuation;
	float			LinearAttenuation;
	float			QuadraticAttenuation;
	float			Intensity;
};

// ================================ //
//
cbuffer LightConstants : register( b1 )
{
	float3			AmbientColor;
	uint			NumLights;
	LightParams		Lights[ MAX_LIGHTS ];
}



//--------------------------------------------------------------------------------------
//
struct PhongResult
{
	float3	Diffuse;
	float3	Specular;
};

//--------------------------------------------------------------------------------------
//
float				ComputeLightIntesity			( uint lightIdx, float lightDistance )
{
	float intensity = Lights[ lightIdx ].Intensity;
	float attenuation = 	Lights[ lightIdx ].ConstantAttenuation
								+ Lights[ lightIdx ].LinearAttenuation * lightDistance
								+ Lights[ lightIdx ].QuadraticAttenuation * lightDistance * lightDistance;
	
	return intensity / attenuation;
}

//--------------------------------------------------------------------------------------
//
float3				ComputeDiffuse					( float3 normal, float3 lightDir, uint lightIdx )
{
	float lambertFactor = max( dot( normal, -lightDir ), 0.0f );
	return Lights[ lightIdx ].Color * lambertFactor;
}

//--------------------------------------------------------------------------------------
//
float3				ComputeSpecular					( float3 normal, float3 lightDir, float3 viewDir, uint lightIdx )
{
	float3 reflectedLightDir = reflect( -lightDir, normal );
	float specularCoeff = max( dot( reflectedLightDir, viewDir ), 0.0f );
	
	return Lights[ lightIdx ].Color * pow( specularCoeff, SPECULAR_POWER );
}

//--------------------------------------------------------------------------------------
//
PhongResult			ComputePhongColorPointLight		( float3 normal, float3 viewDir, float3 lightDir, float lightDistance, uint lightIdx )
{	
	float3 colorDiffuse = ComputeDiffuse( normal, lightDir, lightIdx );
	float3 colorSpecular = ComputeSpecular( normal, lightDir, viewDir, lightIdx );
	float lightIntens = ComputeLightIntesity( lightIdx, lightDistance );
	
	PhongResult resultColor;
	resultColor.Diffuse = colorDiffuse * lightIntens;
	resultColor.Specular = colorSpecular * lightIntens;
	
	return resultColor;
}

float 				ComputeSpotAttenuation			( float angle, float3 lightDir, float3 spotDirection )
{
    float minCos = cos( angle );
    float maxCos = ( minCos + 1.0f ) / 2.0f;
    float cosAngle = dot( lightDir, spotDirection );
    return smoothstep( minCos, maxCos, cosAngle ); 
}

//--------------------------------------------------------------------------------------
//
PhongResult			ComputePhongSpotLight			( float3 normal, float3 viewDir, float3 lightDir, float lightDistance, uint lightIdx )
{
	float spotAttenuation = ComputeSpotAttenuation( Lights[ lightIdx ].SpotAngle, lightDir, Lights[ lightIdx ].Direction );
	
	if( spotAttenuation > 0 )
	{
		float3 colorDiffuse = ComputeDiffuse( normal, lightDir, lightIdx );
		float3 colorSpecular = ComputeSpecular( normal, lightDir, viewDir, lightIdx );
		float lightIntens = ComputeLightIntesity( lightIdx, lightDistance );
		lightIntens = lightIntens * spotAttenuation;
		
		PhongResult resultColor;
		resultColor.Diffuse = colorDiffuse * lightIntens;
		resultColor.Specular = colorSpecular * lightIntens;

		return resultColor;
	}

	PhongResult resultColor;
	resultColor.Diffuse = float3( 0.0, 0.0, 0.0 );
	resultColor.Specular = float3( 0.0, 0.0, 0.0 );
	
	return resultColor;
}

//--------------------------------------------------------------------------------------
//
PhongResult			ComputePhongDirectionalLight	( float3 normal, float3 viewDir, uint lightIdx )
{
	PhongResult resultColor;

	resultColor.Diffuse = ComputeDiffuse( normal, Lights[ lightIdx ].Direction, lightIdx );
	resultColor.Specular = ComputeSpecular( normal, Lights[ lightIdx ].Direction, viewDir, lightIdx );
	
	return resultColor;
}

//--------------------------------------------------------------------------------------
//
PhongResult			ComputeLightPhong				( float3 worldPosition, float3 normal, float3 viewDir )
{
	PhongResult resultColor;
	
	for( unsigned int i = 0; i < NumLights; ++i )
	{
		PhongResult phongColor;
	
		if( Lights[ i ].LightType == DIRECTIONAL_LIGHT )
		{
			phongColor = ComputePhongDirectionalLight( normal, viewDir, i );
		}
		else
		{
			// Computing lights in world space.
			float3 lightDir = worldPosition - Lights[ i ].Position;
			float lightDistance = length( lightDir );
			lightDir = lightDir / lightDistance;
		
			if( Lights[ i ].LightType == POINT_LIGHT )
			{
				phongColor = ComputePhongColorPointLight( normal, viewDir, lightDir, lightDistance, i );
			}
			else if( Lights[ i ].LightType == SPOT_LIGHT )
			{
				phongColor = ComputePhongSpotLight( normal, viewDir, lightDir, lightDistance, i );
			}
		}
		
		resultColor.Diffuse += phongColor.Diffuse;
		resultColor.Specular += phongColor.Specular;
	}
	
	return resultColor;
}

//--------------------------------------------------------------------------------------
// This function computes final color. You can use ComputeLightPhong instead, if you want
// apply texturing or some other tricks.
//
// Remember before calling this function.
//	float3 normal = normalize( Normal );
// float3 viewDir = normalize( WorldPosition - CameraPosition );
float4				ComputePhongLightResultColor		( float3 worldPosition, float3 normal, float3 viewDir, float3 color )
{
	float4 resultColor;
	resultColor.w = 1.0;
	resultColor.xyz = AmbientColor * color;
	//resultColor.xyz += Emissive;

	PhongResult phongResult = ComputeLightPhong( worldPosition, normal, viewDir );
	
	resultColor.xyz += phongResult.Diffuse.xyz * color;
	resultColor.xyz += phongResult.Specular;			// Specular is always white for now.
	
	return resultColor;
}






