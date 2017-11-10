
#include "../RaycasterGPU/Declarations.hlsl"		// This is hack. Raycaster.hlsl can't include this file when it's nested.
#include "../Tools/MatrixInverse.hlsl"
#include "../RaycasterGPU/Raycaster.hlsl"
#include "../Shading/PhongShading.hlsl"



#define NUM_WEIGHTS 4
#define WEIGHTS_PER_TRIANGLE ( NUM_WEIGHTS * 3 )
#define INVALID_IDX 255
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
}


// ================================ //
//
struct OutputGS
{
	float4					Position : SV_POSITION;
	linear float3			WorldPosition : TEXCOORD0;
	nointerpolation uint	BlendIdx[ WEIGHTS_PER_TRIANGLE ] : BLENDINDICES;
	linear float			BlendWeights[ WEIGHTS_PER_TRIANGLE ] : BLENDWEIGHT;
};



// ================================ //
//
float4 main( OutputGS input ) : SV_TARGET
{
	const float offsetRay = 0.05;

	float4 resultColor = float4( 0.0, 0.0, 0.0, 0.0 );

	// Create transformation matrix for this pixel.
	matrix transformMatrix = input.BlendWeights[ 0 ] * BoneTransform[ input.BlendIdx[ 0 ] ];
	for( int i = 1; i < WEIGHTS_PER_TRIANGLE; ++i )
	{
		float weight = input.BlendWeights[ i ];
		if( weight == 0.0 )
			break;

		transformMatrix += weight * BoneTransform[ input.BlendIdx[ i ] ];
	}

	transformMatrix = Inverse( transformMatrix );

	float3 direction = normalize( input.WorldPosition - CameraPosition );
	float3 position = mul( float4( input.WorldPosition, 1.0f ), transformMatrix ).xyz;
	direction = mul( float4( direction, 0.0f ), transformMatrix ).xyz;

	// Mesh was scaled and centered but bones transformation not.
	// We must inverse these transformations.
	position = position + Translate;
	position = position * Scale;

	// Note: The octree is assumed to reside at coordinates [1, 2]. Our Shell mesh is in center of coordinates system
	// and has bounding box of size 1.0. We must move start position.
	// We don't support shell meshes not in center just yet.
	position += float3( 1.5f, 1.5f, 1.5f );
	position -= direction * offsetRay;

	// Paint all pixels inside shell mesh.
	resultColor = float4( 0.2, 0.2, 0.2, 1.0 );

	RaycasterResult result = RaycastingCore( input.Position, position, direction );

	if( result.VoxelIdx != 0 )
	{
		resultColor = float4( 1.0, 1.0, 1.0, 1.0 );

		OctreeLeaf leaf = GetResultLeafNode( result.VoxelIdx );
		VoxelAttributes attributes = GetLeafAttributes( leaf );

		//return resultColor;
		return float4( attributes.Color ) / 255.0f;
	}

	return resultColor;
}


