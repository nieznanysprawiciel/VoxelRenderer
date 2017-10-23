

// ================================ //
//
struct OutputVS
{
	linear float4			Position : SV_POSITION;
	linear float4			Color : COLOR;
};

// ================================ //
//
float4 main( OutputVS input ) : SV_TARGET
{
	return input.Color;
}
