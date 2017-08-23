#ifndef _CAMERA_HLSL
#define _CAMERA_HLSL

#include "Declarations.hlsl"


float3		ComputeRayPosition		( CameraData camera, int screenX, int screenY );
float3		ComputeRayDirection		( CameraData camera, int screenX, int screenY );


#endif
