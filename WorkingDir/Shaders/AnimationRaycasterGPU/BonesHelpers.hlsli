

#define MAX_BONES 200



// ================================ //
//
cbuffer BonesTransforms : register( b1 )
{
	matrix			BoneTransform[ MAX_BONES ];
}


// ================================ //
//
float4		ComputeBonesTransform				( float4 toTransform, uint4 indicies, float4 weights )
{
	float4 newVector = mul( toTransform, BoneTransform[ indicies.x ] ) * weights.x;
	newVector += mul( toTransform, BoneTransform[ indicies.y ] ) * weights.y;
	newVector += mul( toTransform, BoneTransform[ indicies.z ] ) * weights.z;
	newVector += mul( toTransform, BoneTransform[ indicies.w ] ) * weights.w;

	return newVector;
}

// ================================ //
//
float4		ComputeBonesPositionsTransform		( float4 toTransform, uint4 indicies, float4 weights )
{
	float4 newVector = ComputeBonesTransform( toTransform, indicies, weights );
	newVector.w = 1.0f;
		
	return newVector;
}


