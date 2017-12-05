
#include "../RaycasterGPU/Declarations.hlsl"		// This is hack. Raycaster.hlsl can't include this file when it's nested.
#include "../Tools/MatrixInverse.hlsl"
#include "../RaycasterGPU/Raycaster.hlsl"
#include "../Shading/PhongShading.hlsl"
#include "AnimHelpers.hlsli"



// ================================ //
//
struct OutputNormGS
{
	float4					Position : SV_POSITION;
	linear float3			WorldPosition : TEXCOORD0;
	linear float3			ModelPosition : TEXCOORD1;
	linear float3			Barycentric : TEXCOORD2;
	linear float3			Normal : TEXCOORD3;
	nointerpolation uint4	BlendIdx[ NUM_TRIANGLES ] : BLENDINDICES;
	nointerpolation float4	BlendWeights[ NUM_TRIANGLES ] : BLENDWEIGHT;
};



// ================================ //
//
float4 main( OutputNormGS input ) : SV_TARGET
{
	const float offsetRay = OffsetShell;

	float4 resultColor = float4( 0.0, 0.0, 0.0, 0.0 );

	float3 direction = input.Normal;
	float3 position = input.ModelPosition;

	// If shell mesh should be larger then underlying voxel model. If it isn't, we can offset ray to avoid collision misses.
	position -= direction * offsetRay;

	if( !any( isnan( position ) ) && !any( isnan( direction ) ) )
	{
		RaycasterResult result = RaycastingCore( position, direction );

		if( result.VoxelIdx != 0 )
		{
			resultColor = float4( 1.0, 1.0, 1.0, 1.0 );

			OctreeLeaf leaf = GetResultLeafNode( result.VoxelIdx );
			VoxelAttributes attributes = GetLeafAttributes( leaf );

			//return resultColor;
			return float4( attributes.Color ) / 255.0f;
		}
	}

	// Incase of voxel misses don't paint enything on screen.
	discard;
	return resultColor;
}


