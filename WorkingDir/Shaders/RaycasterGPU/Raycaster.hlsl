#ifndef _RAYCASTER_HLSL
#define _RAYCASTER_HLSL


#ifndef _DECLARATIONS_HLSL
	#include "Declarations.hlsl"
#endif


// ================================ //
//
struct StackOperation
{
	StackElement		Element;
	uint				Idx;
};




void		InitRaycasting			( float3 position, float3 direction, inout RaycasterContext rayCtx );


RaycasterResult		CastRay			( RaycasterContext rayCtx );


ChildFlag			AdvanceStep		( inout RaycasterContext rayCtx, float3 corner, float tLeave );
uint				PrePopStep		( inout RaycasterContext rayCtx, ChildFlag childIdxChange );
void				PostPopStep		( inout RaycasterContext rayCtx, StackElement stackElement );
StackOperation		PushStep		( inout RaycasterContext rayCtx, float3 corner, float tVoxelMax, ChildFlag childShift );

uint		FindNewHierarchyLevel	( inout float3 position, float scaleExp, ChildFlag childIdxChange );
uint		ComputeChildOffset		( RaycasterContext rayCtx, OctreeNode node, ChildFlag childShift );

bool		IsEmpty					( OctreeNode node );
bool		IsRayOutside			( ChildFlag childIdx, ChildFlag childIdxChange );
uint		CountNodesBefore		( ChildFlag childShift, uint childMask );





// ================================ //
//
float				copysignf			( float value, float signParam )
{
	if( signParam > 0.0 )
		return abs( value );
	else
		return -abs( value );
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
float3				ParamLine			( float3 coords, float3 tCoeff, float3 tBias )
{
	return tCoeff * coords - tBias;
}

// ================================ //
//
float3				ParamLine			( float3 coords, RaycasterContext rayCtx )
{
	return ParamLine( coords, rayCtx.tCoeff, rayCtx.tBias );
}

// ================================ //
//
float				Min					( float3 coords )
{
	return min( min( coords.x, coords.y ), coords.z );
}

// ================================ //
//
bool				ExistsChild			( OctreeNode node, ChildFlag childShift )
{
	return ( ChildMask( node ) & ( 0x80 >> childShift ) ) != 0;
}

// ================================ //
//
bool				IsLeaf				( OctreeNode node )
{
	uint mask = ( 0x1 << 31 );
	return ( node & mask ) != 0;
}

// ================================ //
//
bool				IsIndirectPointer	( OctreeNode node )
{
	return ( node & ( 0x1 << 30 ) ) != 0;
}

// ================================ //
//
int					HighestBitPos		( int diffs )
{
	// Note: Conversion to float normalizes this float. We can read position of highest bit from exponent.
	// So we shift this float by 23 bits and subtract 127 (exponent bias).
	return ( asuint( (float)diffs ) >> 23 ) - 127;
}


// ================================ //
//
void				InitRaycasting		( float3 position, float3 direction, inout RaycasterContext rayCtx )
{
	//const float epsilon = exp2( -(float)CAST_STACK_DEPTH );
	const float epsilon = 1e-10;

	rayCtx.RayStartPosition = position;
	rayCtx.RayDirection = direction;
	rayCtx.ChildDescriptor = 0;

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

	BlockDescriptor blockDesc = GetBlockDescriptor( 0 );

	rayCtx.Current = blockDesc.RootNodeOffset;
	rayCtx.Scale = CAST_STACK_DEPTH - 1;
	rayCtx.ScaleExp = 0.5f;
	rayCtx.ChildIdx = 0;
	rayCtx.Position.xyz = float3( 1.0f, 1.0f, 1.0f );

	// We must decide in which voxel we are in first step. Check which half of voxel is intersected along each axis.
	if( ParamLineX( 1.5f, rayCtx ) > rayCtx.tMin ) rayCtx.ChildIdx ^= 1, rayCtx.Position.x = 1.5f;
	if( ParamLineY( 1.5f, rayCtx ) > rayCtx.tMin ) rayCtx.ChildIdx ^= 2, rayCtx.Position.y = 1.5f;
	if( ParamLineZ( 1.5f, rayCtx ) > rayCtx.tMin ) rayCtx.ChildIdx ^= 4, rayCtx.Position.z = 1.5f;
}

// ================================ //
//
RaycasterResult		CastRay				( RaycasterContext rayCtx )
{
	// Note: In CPU version stack is part of RaycasterContext. It's not posible in shader because array can't be passed to functions.
	// Thats why in this code there're some horible things.
	StackElement NodesStack[ CAST_STACK_DEPTH ];

	while( rayCtx.Scale < CAST_STACK_DEPTH )
	{
		if( rayCtx.ChildDescriptor == 0 )
			rayCtx.ChildDescriptor = GetNode( rayCtx.Current );

		// Terminate.
		if( IsLeaf( rayCtx.ChildDescriptor ) )
			break;

		// Compute t-value in which ray leaves current voxel.
		float3 corner = ParamLine( rayCtx.Position, rayCtx );
		float tLeave = Min( corner );

		ChildFlag childShift = rayCtx.ChildIdx ^ rayCtx.OctantMask; // permute child slots based on the mirroring

		if( ExistsChild( rayCtx.ChildDescriptor, childShift ) && rayCtx.tMin <= rayCtx.tMax )
		{
			float tVoxelMax = min( rayCtx.tMax, tLeave );

			if( rayCtx.tMin <= tVoxelMax )
			{
				StackOperation op = PushStep( rayCtx, corner, tVoxelMax, childShift );
				
				NodesStack[ op.Idx ] = op.Element;
				continue;
			}
		}

		ChildFlag childIdxChange = AdvanceStep( rayCtx, corner, tLeave );

		if( IsRayOutside( rayCtx.ChildIdx, childIdxChange ) )
		{
			uint idx = PrePopStep( rayCtx, childIdxChange );
			PostPopStep( rayCtx, NodesStack[ idx ] );
		}
	}

	if( rayCtx.Scale >= CAST_STACK_DEPTH )
		rayCtx.tMin = 2.0f;

	if( ( rayCtx.OctantMask & 1 ) == 0 ) rayCtx.Position.x = 3.0f - rayCtx.ScaleExp - rayCtx.Position.x;
	if( ( rayCtx.OctantMask & 2 ) == 0 ) rayCtx.Position.y = 3.0f - rayCtx.ScaleExp - rayCtx.Position.y;
	if( ( rayCtx.OctantMask & 4 ) == 0 ) rayCtx.Position.z = 3.0f - rayCtx.ScaleExp - rayCtx.Position.z;

	RaycasterResult result;
	result.Depth = rayCtx.tMin;		//rayCtx.tMin - rayCtx.tCubeMin;;
	result.VoxelIdx = rayCtx.Current;

	return result;
}

// ================================ //
//
ChildFlag		AdvanceStep			( inout RaycasterContext rayCtx, float3 corner, float tLeave )
{
	// ADVANCE
	// Step along the ray.

	// Check which voxel boundaries we crossed and set proper flag. Advance position by current voxel size.
	ChildFlag childIdxChange = 0;
	if( corner.x <= tLeave ) childIdxChange ^= 1, rayCtx.Position.x -= rayCtx.ScaleExp;
	if( corner.y <= tLeave ) childIdxChange ^= 2, rayCtx.Position.y -= rayCtx.ScaleExp;
	if( corner.z <= tLeave ) childIdxChange ^= 4, rayCtx.Position.z -= rayCtx.ScaleExp;

	// Update active t-span and flip bits of the child slot index.

	rayCtx.tMin = tLeave;
	rayCtx.ChildIdx ^= childIdxChange;

	return childIdxChange;
}

// ================================ //
//
uint			PrePopStep			( inout RaycasterContext rayCtx, ChildFlag childIdxChange )
{
	// POP
	// Find the highest differing bit between the two positions.

	rayCtx.Scale = FindNewHierarchyLevel( rayCtx.Position, rayCtx.ScaleExp, childIdxChange );
	rayCtx.ScaleExp = asfloat( ( rayCtx.Scale - CAST_STACK_DEPTH + 127 ) << 23 ); // exp2f(scale - s_max)

	return rayCtx.Scale;
}


// ================================ //
//
void			PostPopStep			( inout RaycasterContext rayCtx, StackElement stackElement )
{
	// Restore parent voxel from the stack.
	rayCtx.Current = stackElement.Node;
	rayCtx.tMax = stackElement.tMax;
				

	// Round cube position and extract child slot index.

	int shx = asuint( rayCtx.Position.x ) >> rayCtx.Scale;
	int shy = asuint( rayCtx.Position.y ) >> rayCtx.Scale;
	int shz = asuint( rayCtx.Position.z ) >> rayCtx.Scale;
	rayCtx.Position.x = asfloat( shx << rayCtx.Scale );
	rayCtx.Position.y = asfloat( shy << rayCtx.Scale );
	rayCtx.Position.z = asfloat( shz << rayCtx.Scale );
	rayCtx.ChildIdx  = ( shx & 1 ) | ( ( shy & 1 ) << 1 ) | ( ( shz & 1 ) << 2 );

	// Prevent same parent from being stored again and invalidate cached child descriptor.
	rayCtx.ChildDescriptor = 0;
}

// ================================ //
//
StackOperation		PushStep				( inout RaycasterContext rayCtx, float3 corner, float tVoxelMax, ChildFlag childShift )
{
	float halfVoxel = rayCtx.ScaleExp * 0.5f;				// Half of current node cube dimmension.

	// This line computes intersection with center of current voxel. Write full equation to see why this works.
	float3 tCenter = ParamLine( float3( halfVoxel, halfVoxel, halfVoxel ), rayCtx.tCoeff, -corner );

	//PushOnStack( rayCtx.NodesStack, rayCtx.Scale, rayCtx.Current, rayCtx.tMax );
	
	StackOperation op;
	op.Idx = rayCtx.Scale;
	op.Element.tMax = rayCtx.tMax;
	op.Element.Node = rayCtx.Current;

	rayCtx.Current = ComputeChildOffset( rayCtx, rayCtx.ChildDescriptor, childShift );


	// Select child voxel that the ray enters first.
	rayCtx.ChildIdx = 0;
	rayCtx.Scale--;
	rayCtx.ScaleExp = halfVoxel;

	if( tCenter.x > rayCtx.tMin ) rayCtx.ChildIdx ^= 1, rayCtx.Position.x += rayCtx.ScaleExp;
	if( tCenter.y > rayCtx.tMin ) rayCtx.ChildIdx ^= 2, rayCtx.Position.y += rayCtx.ScaleExp;
	if( tCenter.z > rayCtx.tMin ) rayCtx.ChildIdx ^= 4, rayCtx.Position.z += rayCtx.ScaleExp;

	// Update active t-span and invalidate cached child descriptor.

	rayCtx.tMax = tVoxelMax;
	rayCtx.ChildDescriptor = 0;

	return op;
}

//====================================================================================//
//			Raycasting skeleton	
//====================================================================================//


// ================================ //
//
RaycasterResult				RaycastingCore			( float3 position, float3 direction )
{
	RaycasterContext rayCtx;

	InitRaycasting( position, direction, rayCtx );
	return CastRay( rayCtx );
}


//====================================================================================//
//			Other helper functions	
//====================================================================================//



// ================================ //
//
bool		IsRayOutside		( ChildFlag childIdx, ChildFlag childIdxChange )
{
	// If the bit flips disagree with the ray direction, ray is outside of current voxel.
	return ( childIdx & childIdxChange ) != 0;
}


// ================================ //
//
uint		CountNodesBefore	( ChildFlag childShift, uint childMask )
{
	// childShift represents index of child voxel in childMask counting from highest bit.
	uint mask = 0x7F;											// Selects all bits except first in childMask.
	uint nodesBefore = ( childMask << childShift ) & mask;		// Shift all children bits in such way that our current xhild will be on first place.

	return countbits( nodesBefore );
}


// ================================ //
//
uint		FindNewHierarchyLevel	( inout float3 position, float scaleExp, ChildFlag childIdxChange )
{
	// During tree traversal, ray can stay inside the same voxel and step to neighbor child or it can leave voxel.
	// In second case we must go up in the voxels hierarchy (sometimes even few levels up).
	// This function finds, how many octree levels we must leave.
	// This code checks differing bits in float representation of position before step and after.
	// Remember that positions can be only negatives powers of 1.
	// The highest bit will say how much we must go upside.
	uint differingBits = 0;
	if( ( childIdxChange & 1 ) != 0 ) differingBits |= asuint( position.x ) ^ asuint( position.x + scaleExp );
	if( ( childIdxChange & 2 ) != 0 ) differingBits |= asuint( position.y ) ^ asuint( position.y + scaleExp );
	if( ( childIdxChange & 4 ) != 0 ) differingBits |= asuint( position.z ) ^ asuint( position.z + scaleExp );

	return HighestBitPos( differingBits );
}

// ================================ //
//
uint		ComputeChildOffset		( RaycasterContext rayCtx, OctreeNode node, ChildFlag childShift )
{
	// Find child descriptor corresponding to the current voxel.
	uint childOffset = 0;
	if( IsIndirectPointer( node ) ) // far
	{
		childOffset = GetIndirectPtr( rayCtx, node );
	}
	else
	{
		childOffset = rayCtx.Current + ChildPackPtr( node );
	}

	childOffset += CountNodesBefore( childShift, ChildMask( node ) );
	return childOffset;
}

#endif
