#ifndef _RAYCASTER_WITH_CAMERA_HLSL
#define _RAYCASTER_WITH_CAMERA_HLSL

#include "Declarations.hlsl"
#include "Raycaster.hlsl"
#include "Camera.hlsl"



// ================================ //
// 
RaycasterResult		RaycastingRange	( float4 screenSpace, CameraData cameraInput, float maxRaycastPath )
{
	float3 direction = ComputeRayDirection( cameraInput, screenSpace.x, screenSpace.y );
	float3 position = ComputeRayPosition( cameraInput, screenSpace.x, screenSpace.y );

	return RaycastingCore( position, direction );
}


// ================================ //
//
RaycasterResult		Raycasting		( float4 screenSpace, CameraData cameraInput )
{
	return RaycastingRange( screenSpace, cameraInput, -1.0f );
}


#endif
