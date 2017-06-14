#include "PrintOctreeRaycaster.h"




namespace vr
{


// ================================ //
//
void			PrintOctreeRaycaster::RaycasterThreadImpl		( ThreadData& data, Size threadNumber )
{
	uint32 octreeLevel = 2;
	uint32 viewDepth = 0;

	uint32 gridSize = (uint32)pow( 2, octreeLevel );
	uint32 rectSize = std::min( m_width, m_height );
	uint32 rectOffsetX = m_width > m_height ? ( m_width - rectSize ) / 2 : 0;
	uint32 rectOffsetY = m_width < m_height ? ( m_height - rectSize ) / 2 : 0;
	uint32 voxelPixSize = rectSize / gridSize;

	// For all pixels in range for this thread.
	for( uint32 pix = data.StartRange; pix < data.EndRange; ++pix )
	{
		RaycasterContext rayCtx;
		rayCtx.Octree = data.Octree;
		rayCtx.NodesStack.resize( rayCtx.Octree->GetMaxDepth() + 1 );
		rayCtx.Current = rayCtx.Octree->GetRootNodeOffset();
		rayCtx.ChildIdx = 0;

		uint32 yPix = pix / m_width;
		uint32 xPix = pix % m_width;

		// This is screen area where's nothing to print.
		if( xPix < rectOffsetX || yPix < rectOffsetY || xPix > m_width - rectOffsetX || yPix > m_height - rectOffsetY )
		{
			data.Buffer[ pix ] = DirectX::PackedVector::XMCOLOR( 0.0f, 0.8f, 1.0f, 0.0f );
			continue;
		}

		uint32 voxelCoordX = ( xPix - rectOffsetX ) / voxelPixSize;
		uint32 voxelCoordY = ( yPix - rectOffsetY ) / voxelPixSize;
		uint32 voxelCoordZ = viewDepth;

		bool fill = IsFilledVoxel( rayCtx, octreeLevel, voxelCoordX, voxelCoordY, voxelCoordZ );

		if( fill )
			data.Buffer[ pix ] = DirectX::PackedVector::XMCOLOR( 1.0, 0.0, 0.0, 0.0 );
		else
			data.Buffer[ pix ] = DirectX::PackedVector::XMCOLOR( 0.0, 0.0, 0.0, 0.0 );
	}

}

// ================================ //
//
bool			PrintOctreeRaycaster::IsFilledVoxel				( RaycasterContext& rayCtx, uint32 octreeLevel, uint32 voxelX, uint32 voxelY, uint32 voxelZ )
{
	auto rootOffset = rayCtx.Octree->GetRootNodeOffset();
	uint32 curOffset = rootOffset;
	uint32 levelGridSize = (uint32)pow( 2, octreeLevel );

	// On each level decide if we should go in positive or negative direction.
	for( uint32 i = 0; i < octreeLevel; ++i )
	{
		// If voxelN is greater then half of grid size we move in positive direction.
		// Note: levelGridSize isn't size of grid on level i. What we are doing here is actually aproximating
		// voxelN with next binary numbers (since octree is binary tree).
		levelGridSize = levelGridSize / 2;
		ChildFlag childMask = 0;

		if( voxelX >= levelGridSize )	childMask |= 1, voxelX -= levelGridSize;
		if( voxelY >= levelGridSize )	childMask |= 2, voxelY -= levelGridSize;
		if( voxelZ >= levelGridSize )	childMask |= 4, voxelZ -= levelGridSize;

		auto curNode = &rayCtx.Octree->GetNode( curOffset );
		if( !ExistsChild( curNode, childMask ) )
			return false;

		curOffset = ComputeChildOffset( rayCtx, curNode, childMask );
	}


	return true;
}

}	// vr




