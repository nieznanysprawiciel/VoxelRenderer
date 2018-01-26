
#include "../RaycasterGPU/Declarations.hlsl"		// This is hack. Raycaster.hlsl can't include this file when it's nested.
#include "../Tools/MatrixInverse.hlsl"
#include "../RaycasterGPU/Raycaster.hlsl"
#include "../Shading/PhongShading.hlsl"
#include "AnimHelpers.hlsli"


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
float4 main( OutputVS input ) : SV_TARGET
{
	const float offsetRay = OffsetShell + 0.01;

	float4 resultColor = float4( 0.4, 0.4, 0.4, 1.0 );

	float3 versorX = normalize( input.VersorX );
	float3 versorY = normalize( input.VersorY );
	float3 versorZ = normalize( input.VersorZ );

	float3x3 changeBaseMatrix = { versorX, versorY, versorZ };

	// Position is already computed since we need position in model space.
	float3 direction = input.WorldPosition - CameraPosition;
	float3 position = input.ModelPosition;

	direction = mul( direction, changeBaseMatrix );

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


