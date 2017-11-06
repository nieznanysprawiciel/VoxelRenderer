#include "../Tools/MatrixInverse.hlsl"




#define NUM_WEIGHTS 4
#define WEIGHTS_PER_TRIANGLE ( NUM_WEIGHTS * 3 )
#define INVALID_IDX 255
#define MAX_BONES 200


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
struct OutputGS
{
	float4					Position : SV_POSITION;
	nointerpolation uint	BlendIdx[ WEIGHTS_PER_TRIANGLE ] : BLENDINDICES;
	linear float			BlendWeights[ WEIGHTS_PER_TRIANGLE ] : BLENDWEIGHT;
};

// ================================ //
//
cbuffer BonesTransforms : register( b1 )
{
	matrix			BoneTransform[ MAX_BONES ];
}


// ================================ //
//
float4 main( OutputVS input ) : SV_TARGET
{



	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}


