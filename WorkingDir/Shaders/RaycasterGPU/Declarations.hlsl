#ifndef _DECLARATIONS_HLSL
#define _DECLARATIONS_HLSL



#define CAST_STACK_DEPTH        23



ByteAddressBuffer Octree : register( t0 );


typedef uint ChildFlag;
typedef uint OctreeNode;
typedef uint OctreeLeaf;

#define OCTREE_NODE_SIZE		4



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
	float				Fov;
	float3				Direction;
	float				Width;
	float3				UpVector;
	float				Height;
	float3				RightVector;
	float				ViewportSize;
	float				NearPlane;
	float				FarPlane;
	bool				IsPerspective;
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
uint				ChildPackPtr	( OctreeNode node )
{
	const uint mask = 0x3FFFFF00;
	return ( node & mask ) >> 8;
}

// ================================ //
//
uint				GetNode			( uint idx )
{
	// Note: We address buffer in bytes and pointers are stored as multiple of OctreeNode size.
	return Octree.Load( idx * OCTREE_NODE_SIZE );
}

// ================================ //
//
uint				GetIndirectPtr	( RaycasterContext rayCtx, OctreeNode node )
{
	return GetNode( ChildPackPtr( node ) );
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

	// In future we could support multiple descriptors, each on start of new block.
	uint2 desc = Octree.Load2( 0 );

	blockDescriptor.AttributesOffset = desc.x;
	blockDescriptor.RootNodeOffset = desc.y;
	return blockDescriptor;
}

// ================================ //
//
uint				AttributesOffset		( OctreeLeaf leaf )
{
	return leaf & 0x7FFFFFFF;
}

// ================================ //
//
VoxelAttributes		GetAttributes			( uint attributeOffset )
{
	// Note: We address buffer in bytes and pointers are stored as multiple of OctreeNode size.
	float4 attribute = Octree.Load4( attributeOffset * OCTREE_NODE_SIZE );
	uint colorPacked = asuint( attribute.w );
	
	uint4 color;
	color.x = ( colorPacked & 0xFF000000 ) >> 24;
	color.y = ( colorPacked & 0xFF0000 ) >> 16;
	color.z = ( colorPacked & 0xFF00 ) >> 8;
	color.w = ( colorPacked & 0xFF );


	VoxelAttributes attributes;
	attributes.Color = color;
	attributes.Normal = float3( attribute.x, attribute.y, attribute.z );
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
