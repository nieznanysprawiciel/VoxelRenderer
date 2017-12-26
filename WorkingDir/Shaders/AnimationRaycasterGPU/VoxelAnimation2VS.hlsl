
#define MAX_BONES 200



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
cbuffer BonesTransforms : register( b1 )
{
	matrix			BoneTransform[ MAX_BONES ];
}

// ================================ //
//
cbuffer MeshContants : register( b2 )
{
	float3			Translate;
	float			Scale;
	float			OffsetShell;
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
	linear float3			WorldPosition : TEXCOORD0;
	linear float3			ModelPosition : TEXCOORD1;
	nointerpolation uint4	BlendIdx : BLENDINDICES;
	linear float4			BlendWeights : BLENDWEIGHT;
};


// ================================ //
//
OutputVS	main( InputVS input )
{
	OutputVS output = (OutputVS)0;

	float4 position = mul( input.Position, BoneTransform[ input.BlendIdx.x ] ) * input.BlendWeights.x;
	position += mul( input.Position, BoneTransform[ input.BlendIdx.y ] ) * input.BlendWeights.y;
	position += mul( input.Position, BoneTransform[ input.BlendIdx.z ] ) * input.BlendWeights.z;
	position += mul( input.Position, BoneTransform[ input.BlendIdx.w ] ) * input.BlendWeights.w;

	position.w = 1.0f;

	output.WorldPosition = position.xyz;
	output.Position = mul( position, ViewMatrix );
	output.Position = mul( output.Position, ProjectionMatrix );
	output.BlendIdx = input.BlendIdx;
	output.BlendWeights = input.BlendWeights;

	output.ModelPosition = input.Position.xyz;
	output.ModelPosition += Translate;
	output.ModelPosition = output.ModelPosition * Scale;

	return output;
}
