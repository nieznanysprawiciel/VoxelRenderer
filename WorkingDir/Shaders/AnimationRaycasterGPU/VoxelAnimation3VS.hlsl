#include "../Tools/MatrixInverse.hlsl"
#include "AnimHelpers.hlsli"




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
struct OutputVS
{
	linear float4			Position : SV_POSITION;
	linear float3			WorldPosition : TEXCOORD0;
	linear float3			ModelPosition : TEXCOORD1;
	nointerpolation uint4	BlendIdx : BLENDINDICES;
	linear float4			BlendWeights : BLENDWEIGHT;
	linear float3			Normal : NORMAL;
};



// ================================ //
//
OutputVS	main( InputVS input )
{
	OutputVS output = (OutputVS)0;

	float4 position = ComputeBonesPositionsTransform( input.Position, input.BlendIdx, input.BlendWeights );
	float4 normal = ComputeBonesTransform( float4( input.Normal, 0.0f ), input.BlendIdx, input.BlendWeights );

	output.WorldPosition = position.xyz - normal.xyz * OffsetShell;
	output.Position = mul( position, ViewMatrix );
	output.Position = mul( output.Position, ProjectionMatrix );
	output.BlendIdx = input.BlendIdx;
	output.BlendWeights = input.BlendWeights;

	output.ModelPosition = input.Position.xyz;
	output.ModelPosition += Translate;
	output.ModelPosition = output.ModelPosition * Scale;

	return output;
}

