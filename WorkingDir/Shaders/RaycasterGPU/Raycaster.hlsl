#include "Declarations.hlsl"
#include "Camera.hlsl"


void		InitRaycasting			( float3 position, float3 direction, out RaycasterContext rayCtx );
float4		Raycasting				( CameraData input );
float4		CastRay					( RaycasterContext rayCtx );



// ================================ //
//
float				copysignf			( float value, float signParam )
{
	return abs( value ) * sign( signParam );
}

// ================================ //
//
float				ParamLineX			( float posX, RaycasterContext rayCtx )
{
	return rayCtx.tCoeff.x * posX - rayCtx.tBias.x;
}

// ================================ //
//
float				ParamLineY			( float posY, RaycasterContext rayCtx )
{
	return rayCtx.tCoeff.y * posY - rayCtx.tBias.y;
}

// ================================ //
//
float				ParamLineZ			( float posZ, RaycasterContext rayCtx )
{
	return rayCtx.tCoeff.z * posZ - rayCtx.tBias.z;
}




// ================================ //
//
void				InitRaycasting		( float3 position, float3 direction, out RaycasterContext rayCtx )
{
	const float epsilon = exp2( -(float)CAST_STACK_DEPTH );

	rayCtx.RayStartPosition = position;
	rayCtx.RayDirection = direction;

    // Get rid of small ray direction components to avoid division by zero.

	if( abs( rayCtx.RayDirection.x ) < epsilon ) rayCtx.RayDirection.x = copysignf( epsilon, rayCtx.RayDirection.x );
	if( abs( rayCtx.RayDirection.y ) < epsilon ) rayCtx.RayDirection.y = copysignf( epsilon, rayCtx.RayDirection.y );
	if( abs( rayCtx.RayDirection.z ) < epsilon ) rayCtx.RayDirection.z = copysignf( epsilon, rayCtx.RayDirection.z );

	// Precompute the coefficients of tx(x), ty(y), and tz(z).
	// The octree is assumed to reside at coordinates [1, 2].

	rayCtx.tCoeff.x = 1.0f / -abs( rayCtx.RayDirection.x );
	rayCtx.tCoeff.y = 1.0f / -abs( rayCtx.RayDirection.y );
	rayCtx.tCoeff.z = 1.0f / -abs( rayCtx.RayDirection.z );

	rayCtx.tBias.x = rayCtx.tCoeff.x * rayCtx.RayStartPosition.x;
	rayCtx.tBias.y = rayCtx.tCoeff.y * rayCtx.RayStartPosition.y;
	rayCtx.tBias.z = rayCtx.tCoeff.z * rayCtx.RayStartPosition.z;

	// Choose correct child flag.
    // Select octant mask to mirror the coordinate system so
    // that ray direction is negative along each axis.

	rayCtx.OctantMask = 7;
	if( rayCtx.RayDirection.x > 0.0f ) rayCtx.OctantMask ^= 1, rayCtx.tBias.x = ParamLineX( 3.0f, rayCtx );
	if( rayCtx.RayDirection.y > 0.0f ) rayCtx.OctantMask ^= 2, rayCtx.tBias.y = ParamLineY( 3.0f, rayCtx );
	if( rayCtx.RayDirection.z > 0.0f ) rayCtx.OctantMask ^= 4, rayCtx.tBias.z = ParamLineZ( 3.0f, rayCtx );


	// Initialize the active span of t-values.
	rayCtx.tMin = max( max( ParamLineX( 2.0f, rayCtx ), ParamLineY( 2.0f, rayCtx ) ), ParamLineZ( 2.0f, rayCtx ) );
	rayCtx.tMax = min( min( ParamLineX( 1.0f, rayCtx ), ParamLineY( 1.0f, rayCtx ) ), ParamLineZ( 1.0f, rayCtx ) );

	// Enable culling.
	rayCtx.tMin = max( rayCtx.tMin, 0.0f );
	rayCtx.tMax = min( rayCtx.tMax, 10000.0f );
	//rayCtx.tCubeMin = rayCtx.tMin;

	rayCtx.Current = ROOT_OFFSET;
	rayCtx.Scale = CAST_STACK_DEPTH - 1;
	rayCtx.ScaleExp = 0.5f;
	rayCtx.ChildIdx = 0;
	rayCtx.Position.xyz = float3( 1.0f, 1.0f, 1.0f );

	// We must decide in which voxel we are in first step. Check which half of voxel is intersected along each axis.
	if( ParamLineX( 1.5f, rayCtx ) > rayCtx.tMin ) rayCtx.ChildIdx ^= 1, rayCtx.Position.x = 1.5f;
	if( ParamLineY( 1.5f, rayCtx ) > rayCtx.tMin ) rayCtx.ChildIdx ^= 2, rayCtx.Position.y = 1.5f;
	if( ParamLineZ( 1.5f, rayCtx ) > rayCtx.tMin ) rayCtx.ChildIdx ^= 4, rayCtx.Position.z = 1.5f;

	for( int i = 0; i < CAST_STACK_DEPTH; ++i )
	{
		rayCtx.NodesStack[ i ].Node = 0;
		rayCtx.NodesStack[ i ].tMax = 0.0f;
	}
}

//====================================================================================//
//			Raycasting skeleton	
//====================================================================================//


// ================================ //
//
float4				Raycasting				( CameraData input )
{
	RaycasterContext rayCtx;

	int pix = 0;	/// Compute pixel number
	float width = 1000;
	float height = 1000;

	float3 direction = ComputeRayDirection( input, pix % width, height - pix / height );
	float3 position = ComputeRayPosition( input, pix % width, height - pix / height );

	InitRaycasting( position, direction, rayCtx );
	return CastRay( rayCtx );

}


float4 main() : SV_TARGET
{
	CameraData input;
	return Raycasting( input );
}