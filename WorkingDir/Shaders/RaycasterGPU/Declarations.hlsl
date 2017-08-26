#ifndef _DECLARATIONS_HLSL
#define _DECLARATIONS_HLSL



#define CAST_STACK_DEPTH        23
#define ROOT_OFFSET				514

typedef uint ChildFlag;
typedef uint OctreeNode;
typedef uint OctreeLeaf;



// ================================ //
//
struct BlockDescriptor
{
	uint			AttributesOffset;		///< Offset to beginning of attributes block.
	uint			RootNodeOffset;			///< Offset to root node of nodes hierarchy.
};

// ================================ //
//
struct VoxelAttributes
{
	float3				Normal;
	uint4				Color;
};


// ================================ //
//
struct StackElement
{
	uint		Node;
	float		tMax;
};

// ================================ //
//
struct RaycasterContext
{
	OctreeNode						ChildDescriptor;

	float3							RayDirection;
	float3							RayStartPosition;

	float3							tCoeff;
	float3							tBias;
	float3							Position;
	float							tMax;
	float							tMin;

	uint							Current;		///< Current node, we are in.
	ChildFlag						OctantMask;		///< Child bit flipping.
	ChildFlag						ChildIdx;		///< Child in children mask.
	
	int								Scale;
	float							ScaleExp;

	StackElement					NodesStack[ CAST_STACK_DEPTH ];		///< Absolut offsets from beginning of array.
};


// ================================ //
//
struct CameraData
{
	float3				Position;
	float3				Direction;
	float3				UpVector;
	float3				RightVector;
	float				Fov;
	float				Width;
	float				Height;
	float				ViewportSize;
	float				NearPlane;
	float				FarPlane;
	bool				IsPerspective;
};


// ================================ //
//
struct Input
{
	float3			RayDirection;
	float3			RayPosition;
};


// ================================ //
//
struct RaycasterResult
{
	float			Depth;
	uint			VoxelIdx;
};


// ================================ //
//
void				PushOnStack				( RaycasterContext rayCtx, uint idx, uint nodeIdx, float tMax )
{
	StackElement element;
	element.Node = nodeIdx;
	element.tMax = tMax;

	rayCtx.NodesStack[ idx ] = element;
}

// ================================ //
//
StackElement		ReadStack				( RaycasterContext rayCtx, uint idx )
{
	return rayCtx.NodesStack[ idx ];
}


// ================================ //
//
ChildFlag			ChildMask		( OctreeNode node )
{
	return node & 0xFF;
}

// ================================ //
//
uint				ChildPtrPack	( OctreeNode node )
{
	const uint mask = 0x3FFFFF00;
	return node & mask;
}

// ================================ //
//
uint				GetNode			( uint idx )
{
	return 0;
}

// ================================ //
//
uint				GetIndirectPtr	( RaycasterContext rayCtx, OctreeNode node )
{
	//const vr::OctreeFarPointer& farPointer = Cast< const vr::OctreeFarPointer& >( rayCtx.Octree->GetNode( node->ChildPackPtr ) );
	//return farPointer.Offset;
	return 0;
}

// ================================ //
//
bool				IsEmpty			( OctreeNode node )
{
	// If child mask is zero then node is empty.
	return !ChildMask( node );
}

// ================================ //
//
OctreeLeaf			GetResultLeafNode		( uint voxelIdx )
{
	return GetNode( voxelIdx );
}

// ================================ //
//
BlockDescriptor		GetBlockDescriptor		( OctreeLeaf leaf )
{
	BlockDescriptor blockDescriptor;


	blockDescriptor.AttributesOffset = 0;
	blockDescriptor.RootNodeOffset = ROOT_OFFSET;
	return blockDescriptor;
}

// ================================ //
//
uint				AttributesOffset		( OctreeLeaf leaf )
{
	return leaf & ( 0x1 << 31 );
}

// ================================ //
//
VoxelAttributes		GetAttributes			( uint attributeOffset )
{
	VoxelAttributes attributes;
	attributes.Color = uint4( 255, 255, 255, 255 );
	attributes.Normal = float3( 1.0, 0.0, 0.0 );
	return attributes;
}

// ================================ //
//
VoxelAttributes		GetLeafAttributes		( OctreeLeaf leaf )
{
	BlockDescriptor blockDescriptor = GetBlockDescriptor( leaf );
	uint attributeOffset = blockDescriptor.AttributesOffset + AttributesOffset( leaf );
	
	return GetAttributes( attributeOffset );
}




#endif
