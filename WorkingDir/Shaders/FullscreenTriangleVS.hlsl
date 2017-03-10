

struct VSQuadOut
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};



VSQuadOut main( uint VertexID : SV_VertexID )
{
	VSQuadOut Out;
	
	// Counter clockwise.
	Out.position.x = (float)( VertexID % 2 ) * 4.0f - 1.0f;
	Out.position.y = (float)( VertexID / 2 ) * 4.0f - 1.0f;
	Out.position.z = 0.0;
	Out.position.w = 1.0f;

	Out.texcoord.x = Out.position.x;
	Out.texcoord.y = Out.position.y;

	return Out;
}


