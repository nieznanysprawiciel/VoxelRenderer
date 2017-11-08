#include "RaycasterWithCamera.hlsl"
#include "../Shading/PhongShading.hlsl"



float4 main( float4 screenSpace : SV_Position ) : SV_TARGET
{
	float3 direction = ComputeRayDirection( CameraInput, screenSpace.x, screenSpace.y );
	float3 position = ComputeRayPosition( CameraInput, screenSpace.x, screenSpace.y );

	RaycasterResult result = RaycastingCore( screenSpace, position, direction );

	if( result.VoxelIdx != 0 )
	{
		OctreeLeaf leaf = GetResultLeafNode( result.VoxelIdx );
		VoxelAttributes attributes = GetLeafAttributes( leaf );

		float3 viewDir = direction;
		float3 voxelPosition = position + viewDir * result.Depth;

		float4 color = float4( attributes.Color ) / 255.0f;
		return ComputePhongLightResultColor( voxelPosition, attributes.Normal, viewDir, color.xyz );
	}
	else
	{
		return float4( 0.0, 0.0, 0.0, 0.0 );
	}
}


