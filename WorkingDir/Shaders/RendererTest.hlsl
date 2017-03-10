


struct VSQuadOut
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD0;
};


VSQuadOut mainVS( uint VertexID : SV_VertexID )
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



float4 mainPS( VSQuadOut input ) : SV_Target
{
	float radius = 0.5 * 0.5;
	if( input.texcoord.x * input.texcoord.x + input.texcoord.y * input.texcoord.y < radius )
	{
		return float4( 1.0, 0.0, 0.0, 1.0 );
	}
	else
	{
		return float4( 0.7, 1.0, 0.0, 1.0 );
	}
}


float4 main( VSQuadOut input ) : SV_Target
{
	return float4( 1.0, 0.0, 1.0, 1.0 );
}

