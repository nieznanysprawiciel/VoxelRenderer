
#include "../RaycasterGPU/Declarations.hlsl"		// This is hack. Raycaster.hlsl can't include this file when it's nested.
#include "../Tools/MatrixInverse.hlsl"
#include "../RaycasterGPU/Raycaster.hlsl"
#include "../Shading/PhongShading.hlsl"
#include "AnimHelpers.hlsli"


// ================================ //
//
struct Output3GS
{
	float4					Position : SV_POSITION;
	linear float3			WorldPosition : TEXCOORD0;
	linear float3			ModelPosition : TEXCOORD1;
	linear float3			Barycentric : TEXCOORD2;
	nointerpolation uint4	BlendIdx[ NUM_TRIANGLES ] : BLENDINDICES;
	nointerpolation float4	BlendWeights[ NUM_TRIANGLES ] : BLENDWEIGHT;
	linear float3			Normal : NORMAL;
};



// ================================ //
//
float4 main( Output3GS input ) : SV_TARGET
{
	const float offsetRay = OffsetShell + 0.01;

	float4 resultColor = float4( 0.4, 0.4, 0.4, 1.0 );

	// Create inverse matricies for all 3 verticies.
	float3x3 vertex1InvMat = ComputeVertexInverseMat( input.BlendWeights[ 0 ], input.BlendIdx[ 0 ] );
	float3x3 vertex2InvMat = ComputeVertexInverseMat( input.BlendWeights[ 1 ], input.BlendIdx[ 1 ] );
	float3x3 vertex3InvMat = ComputeVertexInverseMat( input.BlendWeights[ 2 ], input.BlendIdx[ 2 ] );

	// Position is already computed since we need position in model space.
	// Direction will be computed for each vertex of triangle. We need to transform
	// direction to model space too. In first step we compute transformed directions by taking 
	// 3 vertex matricies. Then we interpolate between these directions using barycentric coordinates.
	float3 direction = input.WorldPosition - CameraPosition;
	float3 position = input.ModelPosition;

	float3 vertex1Direction = normalize( mul( direction, vertex1InvMat ) );
	float3 vertex2Direction = normalize( mul( direction, vertex2InvMat ) );
	float3 vertex3Direction = normalize( mul( direction, vertex3InvMat ) );

	direction = input.Barycentric.x * vertex1Direction + input.Barycentric.y * vertex2Direction + input.Barycentric.z * vertex3Direction;
	direction = normalize( direction );

	// If shell mesh should be larger then underlying voxel model. If it isn't, we can offset ray to avoid collision misses.
	position -= direction * offsetRay;

	if( !any( isnan( position ) ) && !any( isnan( direction ) ) )
	{
		RaycasterResult result = RaycastingCoreRange( position, direction, 2 * offsetRay );

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
	if( !EnableShellDisplay )
		discard;
	return resultColor;
}


