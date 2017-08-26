#include "Raycaster.hlsl"



float4 main() : SV_TARGET
{
	CameraData input;
	RaycasterResult result = Raycasting( input );

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