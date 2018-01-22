#include "BonesHelpers.hlsli"


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
	float			OffsetShell;
}


// ================================ //
//
struct InputVS
{
	float4			Position : POSITION;
	uint4			BlendIdx : BLENDINDICES;
	float4			BlendWeights : BLENDWEIGHT;
	float3			Normal : NORMAL;
};

// ================================ //
//
struct OutputNormVS
{
	float4					Position : SV_POSITION;
	linear float3			ModelPosition : TEXCOORD0;
	linear float3			Normal : TEXCOORD1;
};



// ================================ //
//
OutputNormVS	main( InputVS input )
{
	OutputNormVS output = (OutputNormVS)0;

	float4 position = ComputeBonesPositionsTransform( input.Position, input.BlendIdx, input.BlendWeights );

	output.Position = mul( position, ViewMatrix );
	output.Position = mul( output.Position, ProjectionMatrix );

	output.ModelPosition = input.Position.xyz;
	output.ModelPosition += Translate;
	output.ModelPosition = output.ModelPosition * Scale;

	output.Normal = input.Normal;

	return output;
}

