#include "NormalsRaycaster.h"



namespace vr
{

// ================================ //
//
NormalsRaycaster::NormalsRaycaster()
	:	m_inverse( false )
{}

// ================================ //
//
void			NormalsRaycaster::RaycasterThreadImpl		( ThreadData& data, Size threadNumber )
{
	// For all pixels in range for this thread.
	for( uint32 pix = data.StartRange; pix < data.EndRange; ++pix )
	{
		RaycasterContext rayCtx;
		rayCtx.Octree = data.Octree;
		rayCtx.NodesStack.resize( rayCtx.Octree->GetMaxDepth() + 1 );
		
		// Find starting position
		DirectX::XMFLOAT3 direction = ComputeRayDirection( data.Camera, pix % m_width, m_height - pix / m_width );
		DirectX::XMFLOAT3 position = ComputeRayPosition( data.Camera, pix % m_width, m_height - pix / m_width );
		
		InitRaycasting( position, direction, rayCtx );
		
		// Debug
		if( rayCtx.tMin > rayCtx.tMax )
		{
			data.Buffer[ pix ] = DirectX::PackedVector::XMCOLOR( 1.0, 0.0, 0.0, 0.0 ).c;
			continue;
		}

		CastRay( rayCtx );


		// Shading
		if( rayCtx.Scale >= rayCtx.Octree->GetMaxDepth() )
			data.Buffer[ pix ] = DirectX::PackedVector::XMCOLOR( 0.0, 0.0, 0.0, 0.0 );
		else
		{
			OctreeLeaf leaf = GetResultLeafNode( rayCtx );
			const VoxelAttributes& attributes = GetLeafAttributes( leaf, rayCtx );

			// Fill pixel
			DirectX::PackedVector::XMCOLOR color = DirectX::PackedVector::XMCOLOR( attributes.Normal.x, attributes.Normal.y, attributes.Normal.z, 1.0f );

			if( m_inverse )
			{
				color.r = -color.r;
				color.g = -color.g;
				color.b = -color.b;
			}

			data.Buffer[ pix ] = color;
		}
	}

	// All threads must end before buffer will be furthr processed.
	m_raycastEndBarrier.ArriveAndWait();
}

// ================================ //
//
void			NormalsRaycaster::ProcessInput			( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )
{
	if( keyboard[ sw::input::Keyboard::KEY_LSHIFT ].IsPressed() )
		m_inverse = true;
	else
		m_inverse = false;
}


}