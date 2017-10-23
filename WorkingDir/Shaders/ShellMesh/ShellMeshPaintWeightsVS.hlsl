


// ================================ //
//
cbuffer CameraConstants : register( b0 )
{
	matrix			ViewMatrix;					///< View matrix.
	matrix			ProjectionMatrix;			///< Projection matrix.
	float3			CameraPosition;				///< Position of camera in world space.
}

// ================================ //
//
cbuffer MeshContants : register( b2 )
{
	float3			Translate;
	float			Scale;
}

// ================================ //
//
cbuffer HelperConstants : register( b3 )
{
	uint			ShowBlendWeightsIdx;
}

// ================================ //
//
struct InputVS
{
	float4			Position : POSITION;
	uint4			BlendIdx : BLENDINDICES;
	float4			BlendWeights : BLENDWEIGHT;
};

// ================================ //
//
struct OutputVS
{
	linear float4			Position : SV_POSITION;
	linear float4			Color : COLOR;
};


// ================================ //
//
OutputVS	main( InputVS input )
{
	OutputVS output = (OutputVS)0;

	output.Position = input.Position + float4( Translate, 0.0 );
	output.Position.xyz = output.Position.xyz * Scale;

	output.Position = mul( output.Position, ViewMatrix );
	output.Position = mul( output.Position, ProjectionMatrix );

	float weight = 0.0f;

	if( input.BlendIdx.x == ShowBlendWeightsIdx )
		weight = input.BlendWeights.x;
	else if( input.BlendIdx.y == ShowBlendWeightsIdx )
		weight = input.BlendWeights.y;
	else if( input.BlendIdx.z == ShowBlendWeightsIdx )
		weight = input.BlendWeights.z;
	else if( input.BlendIdx.w == ShowBlendWeightsIdx )
		weight = input.BlendWeights.w;
	else
	{
		output.Color = float4( 0.5f, 0.5f, 0.5f, 1.0f );
		return output;
	}

	if( weight >= 0.5f )
	{
		float lerpFactorG = 1.0f - 2.0f * ( weight - 0.5f );
		output.Color = float4( 1.0f, lerpFactorG, 0.0, 1.0f );
	}
	else
	{
		float lerpFactorR = 2.0f * weight;
		float lerpFactorG = lerpFactorR;
		float lerpFactorB = 1.0f - lerpFactorR;
		output.Color = float4( lerpFactorR, lerpFactorG, lerpFactorB, 1.0f );
	}

	return output;
}

