

// ================================ //
//
struct OutputVS
{
	linear float4			Position : SV_POSITION;
	linear float3			WorldPosition : TEXCOORD0;
	linear float3			ModelPosition : TEXCOORD1;
	nointerpolation uint4	BlendIdx : BLENDINDICES;
	linear float4			BlendWeights : BLENDWEIGHT;
};

#define NUM_WEIGHTS 4
#define NUM_TRIANGLES 3
#define WEIGHTS_PER_TRIANGLE ( NUM_WEIGHTS * 3 )
#define INVALID_IDX 255

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
OutputGS		InitVertex		( OutputVS inVertex )
{
	OutputGS vertex;
	vertex.Position = inVertex.Position;
	vertex.WorldPosition = inVertex.WorldPosition;
	vertex.ModelPosition = inVertex.ModelPosition;
	vertex.Barycentric = float3( 0.0, 0.0, 0.0 );

	for( uint i = 0; i < NUM_TRIANGLES; i++ )
	{
		vertex.BlendIdx[ i ] = uint4( INVALID_IDX, INVALID_IDX, INVALID_IDX, INVALID_IDX );
		vertex.BlendWeights[ i ] = float4( 0.0, 0.0, 0.0, 0.0 );
	}

	return vertex;
}



[maxvertexcount( 3 )]
void main( triangle OutputVS input[ 3 ], inout TriangleStream< OutputGS > output )
{
	OutputGS vertex[ 3 ];

	// Rewrite all attributes from previous step.
	vertex[ 0 ] = InitVertex( input[ 0 ] );
	vertex[ 1 ] = InitVertex( input[ 1 ] );
	vertex[ 2 ] = InitVertex( input[ 2 ] );

	// Each vertex will have whole set of weights and indicies for all 3 triangles.
	for( uint inVertexIdx = 0; inVertexIdx < NUM_TRIANGLES; inVertexIdx++ )
	{
		uint4 blendIndicies = input[ inVertexIdx ].BlendIdx;
		float4 blendWeights = input[ inVertexIdx ].BlendWeights;

		vertex[ 0 ].BlendIdx[ inVertexIdx ] = blendIndicies;
		vertex[ 1 ].BlendIdx[ inVertexIdx ] = blendIndicies;
		vertex[ 2 ].BlendIdx[ inVertexIdx ] = blendIndicies;

		vertex[ 0 ].BlendWeights[ inVertexIdx ] = blendWeights;
		vertex[ 1 ].BlendWeights[ inVertexIdx ] = blendWeights;
		vertex[ 2 ].BlendWeights[ inVertexIdx ] = blendWeights;
	}

	// Set baricentric coords for verticies.
	vertex[ 0 ].Barycentric[ 0 ] = 1.0;
	vertex[ 1 ].Barycentric[ 1 ] = 1.0;
	vertex[ 2 ].Barycentric[ 2 ] = 1.0;

	output.Append( vertex[ 0 ] );
	output.Append( vertex[ 1 ] );
	output.Append( vertex[ 2 ] );
}



