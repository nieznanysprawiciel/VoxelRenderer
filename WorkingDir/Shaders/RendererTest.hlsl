


struct VSQuadOut
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};


VSQuadOut mainVS( uint VertexID : SV_VertexID )
{// ouputs a full screen quad with tex coords
	VSQuadOut Out;
	Out.texcoord = float2( ( VertexID << 1 ) & 2, VertexID & 2 );
	Out.position = float4( Out.texcoord * float2( 2.0f, -2.0f ) + float2( -1.0f, 1.0f ), 0.0f, 1.0f );
	return Out;
}



float4 mainPS( VSQuadOut input ) : COLOR0
{
	return float4( input.position.x, input.position.y, 1.0, 1.0 );
}


float4 main( VSQuadOut input ) : COLOR0
{
	return float4( 1.0, 0.0, 1.0, 1.0 );
}

