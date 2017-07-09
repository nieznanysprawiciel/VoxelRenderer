#include "DepthRaycaster.h"



namespace vr
{

// ================================ //
//
DepthRaycaster::DepthRaycaster()
{}

// ================================ //
//
void			DepthRaycaster::RaycasterThreadImpl		( ThreadData& data, Size threadNumber )
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
		auto rayLength = DirectX::XMVectorSubtract( DirectX::XMLoadFloat3( &rayCtx.RayStartPosition ), DirectX::XMLoadFloat3( &rayCtx.Position ) );
		auto length = DirectX::XMVectorGetW( DirectX::XMVector3Length( rayLength ) );

		float colorValue = rayCtx.Depth;		// Move to range [0.0, 1.0]
		data.Buffer[ pix ] = DirectX::PackedVector::XMCOLOR( colorValue, colorValue, colorValue, 1.0f );
	}

	// All threads must end before buffer will be furthr processed.
	m_raycastEndBarrier.ArriveAndWait();
}

// ================================ //
//
void			DepthRaycaster::ProcessInput			( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )
{}


}