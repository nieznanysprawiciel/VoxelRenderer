

// ================================ //
//
struct OutputVS
{
	linear float4			Position : SV_POSITION;
	linear float3			WorldPosition : TEXCOORD0;
	nointerpolation uint4	BlendIdx : BLENDINDICES;
	linear float4			BlendWeights : BLENDWEIGHT;
};

#define NUM_WEIGHTS 4
#define WEIGHTS_PER_TRIANGLE ( NUM_WEIGHTS * 3 )
#define INVALID_IDX 255

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
OutputGS		InitVertex		( OutputVS inVertex )
{
	OutputGS vertex;
	vertex.Position = inVertex.Position;
	vertex.WorldPosition = inVertex.WorldPosition;

	for( uint i = 0; i < WEIGHTS_PER_TRIANGLE; i++ )
	{
		vertex.BlendIdx[ i ] = INVALID_IDX;
		vertex.BlendWeights[ i ] = 0.0f;
	}

	return vertex;
}


// ================================ //
//
uint		FindMatchingPosition		( in OutputGS vertex, uint blendIdx, inout uint curPos )
{
	for( uint inWeightIdx = 0; inWeightIdx < curPos; inWeightIdx++ )
	{
		if( vertex.BlendIdx[ inWeightIdx ] == blendIdx )
			return inWeightIdx;
	}

	return curPos++;
}


[maxvertexcount( 3 )]
void main( triangle OutputVS input[ 3 ], inout TriangleStream< OutputGS > output )
{
	OutputGS vertex[ 3 ];

	// Initializes all weights with 0.0. This value will be overwritten in next step.
	vertex[ 0 ] = InitVertex( input[ 0 ] );
	vertex[ 1 ] = InitVertex( input[ 1 ] );
	vertex[ 2 ] = InitVertex( input[ 2 ] );

	uint curWeightPos = 0;

	// Since weights will be interpolated, they must posses the same position in vertex.
	// OutputGS contains extended weights array for 12 weights. We must sort them in all verticies.
	for( uint inVertexIdx = 0; inVertexIdx < 3; inVertexIdx++ )
	{
		for( uint inWeightIdx = 0; inWeightIdx < NUM_WEIGHTS; inWeightIdx++ )
		{
			uint blendIdx = input[ inVertexIdx ].BlendIdx[ inWeightIdx ];
			uint matchingPos = FindMatchingPosition( vertex[ 0 ], blendIdx, curWeightPos );

			// Assign blendIdx to all verticies we have under found matchingPos.
			// Note: probably we do this multiple times when all verticies share the same
			// matrix, but it doesn't matter.
			vertex[ 0 ].BlendIdx[ matchingPos ] = blendIdx;
			vertex[ 1 ].BlendIdx[ matchingPos ] = blendIdx;
			vertex[ 2 ].BlendIdx[ matchingPos ] = blendIdx;

			// Assign weight only to vertex, that is currently processed. Some verticies can stay
			// with 0.0 value, what means, that this vertex doesn't reference this matrix.
			// Rasterizer will then properly interpolate between 0.0 and value we already set.
			vertex[ inVertexIdx ].BlendWeights[ matchingPos ] = input[ inVertexIdx ].BlendWeights[ inWeightIdx ];
		}
	}

	output.Append( vertex[ 0 ] );
	output.Append( vertex[ 1 ] );
	output.Append( vertex[ 2 ] );
}



