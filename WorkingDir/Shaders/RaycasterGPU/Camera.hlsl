#ifndef _CAMERA_HLSL
#define _CAMERA_HLSL

#include "Declarations.hlsl"


float3		ComputeRayPosition		( CameraData cameraData, int screenX, int screenY );
float3		ComputeRayDirection		( CameraData cameraData, int screenX, int screenY );


// ================================ //
//
float3		ComputeRayPosition		( CameraData cameraData, int screenX, int screenY )
{
	if( cameraData.IsPerspective )
	{
		return cameraData.Position;
	}
	else
	{
		float aspect = cameraData.Width / cameraData.Height;

		float xFactor = screenX / ( cameraData.Width / 2 ) - 1.0f;
		float yFactor = 1.0f - screenY / ( cameraData.Height / 2 );

		float3 position = cameraData.Position;
		position = cameraData.UpVector * yFactor * cameraData.ViewportSize + position;
		position = cameraData.RightVector * xFactor * aspect * cameraData.ViewportSize + position;

		return position;
	}
}

// ================================ //
//
float3		ComputeRayDirection		( CameraData cameraData, int screenX, int screenY )
{
	if( cameraData.IsPerspective )
	{
		float d = cameraData.Width / ( 2 * tan( radians( cameraData.Fov / 2.0f ) ) );
		float3 cameraAxis = cameraData.Direction * d;

		cameraAxis = cameraData.UpVector * ( cameraData.Height / 2 - screenY ) + cameraAxis;
		cameraAxis = cameraData.RightVector * ( screenX - cameraData.Width / 2 ) + cameraAxis;

		return normalize( cameraAxis );
	}
	else
	{
		return cameraData.Direction;
	}
}

#endif
