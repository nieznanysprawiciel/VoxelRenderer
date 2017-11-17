
#include "../RaycasterGPU/Declarations.hlsl"		// This is hack. Raycaster.hlsl can't include this file when it's nested.
#include "../Tools/MatrixInverse.hlsl"
#include "../RaycasterGPU/Raycaster.hlsl"
#include "../Shading/PhongShading.hlsl"



#define NUM_WEIGHTS 4
#define NUM_TRIANGLES 3
#define WEIGHTS_PER_TRIANGLE ( NUM_WEIGHTS * NUM_TRIANGLES )
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
	linear float3			ModelPosition : TEXCOORD1;
	linear float3			Barycentric : TEXCOORD2;
	nointerpolation uint4	BlendIdx[ NUM_TRIANGLES ] : BLENDINDICES;
	nointerpolation float4	BlendWeights[ NUM_TRIANGLES ] : BLENDWEIGHT;
};



// ================================ //
//
float3x3			Cast3x3						( matrix mat )
{
	return float3x3( mat._11_12_13, mat._21_22_23, mat._31_32_33 );
}


// ================================ //
//
float3x3			ComputeVertexInverseMat		( float4 weights, uint4 indicies )
{
	float3x3 forwardMat =	Cast3x3( BoneTransform[ indicies.x ] ) * weights.x +
							Cast3x3( BoneTransform[ indicies.y ] ) * weights.y +
							Cast3x3( BoneTransform[ indicies.z ] ) * weights.z +
							Cast3x3( BoneTransform[ indicies.w ] ) * weights.w;

	return Inverse3x3( forwardMat );
}


// ================================ //
//
float4 main( OutputGS input ) : SV_TARGET
{
	const float offsetRay = 0.0001;

	float4 resultColor = float4( 0.0, 0.0, 0.0, 0.0 );

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

	float3 vertex1Direction = mul( direction, vertex1InvMat );
	float3 vertex2Direction = mul( direction, vertex2InvMat );
	float3 vertex3Direction = mul( direction, vertex3InvMat );

	direction = input.Barycentric.x * vertex1Direction + input.Barycentric.y * vertex2Direction + input.Barycentric.z * vertex3Direction;
	direction = normalize( direction );

	// If shell mesh should be larger then underlying voxel model. If it isn't, we can offset ray to avoid collision misses.
	position -= direction * offsetRay;

	// Paint all pixels inside shell mesh but outside of voxel model.
	resultColor = float4( 0.2, 0.2, 0.2, 1.0 );

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

	return resultColor;
}


