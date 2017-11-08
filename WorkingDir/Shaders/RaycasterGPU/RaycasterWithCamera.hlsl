#include "Raycaster.hlsl"
#include "Camera.hlsl"



// ================================ //
//
RaycasterResult		Raycasting		( float4 screenSpace, CameraData cameraInput );



// ================================ //
//
RaycasterResult		Raycasting		( float4 screenSpace, CameraData cameraInput )
{
	float3 direction = ComputeRayDirection( cameraInput, screenSpace.x, screenSpace.y );
	float3 position = ComputeRayPosition( cameraInput, screenSpace.x, screenSpace.y );

	return RaycastingCore( screenSpace, position, direction );
}

