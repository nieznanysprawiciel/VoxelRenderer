


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
	nointerpolation uint4	BlendIdx : BLENDINDICES;
	linear float4			BlendWeights : BLENDWEIGHT;
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
	output.BlendIdx = input.BlendIdx;
	output.BlendWeights = input.BlendWeights;

	return output;
}

