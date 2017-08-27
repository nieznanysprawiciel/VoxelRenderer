#include "Raycaster.hlsl"



float4 main( float4 screenSpace : SV_Position ) : SV_TARGET
{
	RaycasterResult result = Raycasting( screenSpace, CameraInput );

	if( result.VoxelIdx != 0 )
	{
		OctreeLeaf leaf = GetResultLeafNode( result.VoxelIdx );
		VoxelAttributes attributes = GetLeafAttributes( leaf );

		return float4( attributes.Color );
	}
	else
	{
		return float4( 0.0, 0.0, 0.0, 0.0 );
	}


}