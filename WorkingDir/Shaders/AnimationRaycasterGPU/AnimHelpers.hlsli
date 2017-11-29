
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
	float			OffsetShell;
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
float3x3			ComputeForwardMatrix		( float4 weights, uint4 indicies )
{
	float3x3 forwardMat =	Cast3x3( BoneTransform[ indicies.x ] ) * weights.x +
							Cast3x3( BoneTransform[ indicies.y ] ) * weights.y +
							Cast3x3( BoneTransform[ indicies.z ] ) * weights.z +
							Cast3x3( BoneTransform[ indicies.w ] ) * weights.w;
	return forwardMat;
}

// ================================ //
//
float3x3			ComputeVertexInverseMat		( float4 weights, uint4 indicies )
{
	float3x3 forwardMat = ComputeForwardMatrix( weights, indicies );
	return Inverse3x3( forwardMat );
}

