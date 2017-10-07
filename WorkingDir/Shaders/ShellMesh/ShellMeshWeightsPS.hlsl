

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
float4 main( OutputVS input ) : SV_TARGET
{
	return float4( input.BlendWeights.x, input.BlendWeights.y, input.BlendWeights.z, 1.0f );
}


