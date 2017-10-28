#include "Raycaster.hlsl"
#include "../Shading/PhongShading.hlsl"



float4 main( float4 screenSpace : SV_Position ) : SV_TARGET
{
	RaycasterResult result = Raycasting( screenSpace, CameraInput );

	if( result.VoxelIdx != 0 )
	{
		OctreeLeaf leaf = GetResultLeafNode( result.VoxelIdx );
		VoxelAttributes attributes = GetLeafAttributes( leaf );

		float3 viewDir = CameraInput.Direction;
		float3 voxelPosition = CameraInput.Position + viewDir * result.Depth;

		float4 color = float4( attributes.Color ) / 255.0f;
		return ComputePhongLightResultColor( voxelPosition, attributes.Normal, viewDir, color.xyz );
	}
	else
	{
		return float4( 0.0, 0.0, 0.0, 0.0 );
	}
}


