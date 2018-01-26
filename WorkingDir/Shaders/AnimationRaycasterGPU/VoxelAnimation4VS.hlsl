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
	linear float3			Normal : NORMAL;

	linear float3			VersorX : TEXCOORD2;
	linear float3			VersorY : TEXCOORD3;
	linear float3			VersorZ : TEXCOORD4;
};



// ================================ //
//
OutputVS	main( InputVS input )
{
	OutputVS output = (OutputVS)0;

	float4 position = ComputeBonesPositionsTransform( input.Position, input.BlendIdx, input.BlendWeights );
	float4 normal = ComputeBonesTransform( float4( input.Normal, 0.0f ), input.BlendIdx, input.BlendWeights );

	output.VersorX = ComputeBonesTransform( float4( 1.0f, 0.0f, 0.0f, 0.0f ), input.BlendIdx, input.BlendWeights ).xyz;
	output.VersorY = ComputeBonesTransform( float4( 0.0f, 1.0f, 0.0f, 0.0f ), input.BlendIdx, input.BlendWeights ).xyz;
	output.VersorZ = ComputeBonesTransform( float4( 0.0f, 0.0f, 1.0f, 0.0f ), input.BlendIdx, input.BlendWeights ).xyz;

	float3 offset = -normal.xyz * OffsetShell;
	position.xyz = position.xyz + offset;

	output.WorldPosition = position.xyz;
	output.Position = mul( position, ViewMatrix );
	output.Position = mul( output.Position, ProjectionMatrix );

	output.ModelPosition = input.Position.xyz;
	output.ModelPosition += Translate;
	output.ModelPosition = output.ModelPosition * Scale;
	output.ModelPosition += offset;

	return output;
}

