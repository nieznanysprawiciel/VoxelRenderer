

struct VSQuadOut
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};


Texture2D sourceRT  : register( t0 );

SamplerState BlitSampler
{
    Filter = FILTER_MIN_MAG_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};


float4	main( VSQuadOut input ) : SV_TARGET
{
	float4 result = sourceRT.Sample( BlitSampler, input.texcoord );

	return result;
}

