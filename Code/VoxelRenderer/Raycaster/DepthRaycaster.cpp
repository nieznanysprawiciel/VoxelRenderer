#include "DepthRaycaster.h"



namespace vr
{

// ================================ //
//
DepthRaycaster::DepthRaycaster()
	:	m_nearClip( 150.0f )
	,	m_farClip( 200.0f )
	,	m_moveFactor( 0.1f )
{}

// ================================ //
//
void			DepthRaycaster::RaycasterThreadImpl		( ThreadData& data, Size threadNumber )
{
	data.Camera->SetFarPlane( m_farClip );
	data.Camera->SetNearPlane( m_nearClip );
	//m_farClip = data.Camera->GetFarPlane();
	//m_nearClip = data.Camera->GetNearPlane();

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
		//DirectX::XMVECTOR rayDir = DirectX::XMLoadFloat3( &rayCtx.RayDirection );
		//DirectX::XMVECTOR paramT = DirectX::XMVectorReplicate( rayCtx.Depth );
		//DirectX::XMVECTOR castVec = DirectX::XMVectorMultiply( rayDir, paramT );

		float range = m_farClip - m_nearClip;
		float depth = 1.0f - ( m_farClip - rayCtx.Depth ) / range;

		auto rayLength = DirectX::XMVectorSubtract( DirectX::XMLoadFloat3( &rayCtx.RayStartPosition ), DirectX::XMLoadFloat3( &rayCtx.Position ) );
		auto length = DirectX::XMVectorGetW( DirectX::XMVector3Length( rayLength ) );

		depth = rayCtx.Depth;	// 1.0f - ( rayCtx.Depth > 1.0f ? rayCtx.Depth : 1.0f );

		float colorValue = depth;		// Move to range [0.0, 1.0]
		data.Buffer[ pix ] = DirectX::PackedVector::XMCOLOR( colorValue, colorValue, colorValue, 1.0f );
	}

	// All threads must end before buffer will be furthr processed.
	m_raycastEndBarrier.ArriveAndWait();
}

// ================================ //
//
void			DepthRaycaster::ProcessInput			( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )
{
	float moveSpeed = ( m_farClip - m_nearClip ) * m_moveFactor;

	if( !keyboard[ sw::input::Keyboard::KEY_LSHIFT ].IsPressed() )
	{
		if( keyboard[ sw::input::Keyboard::KEY_UP ].IsKeyDownEvent() )
		{
			m_farClip += moveSpeed;
		}

		if( keyboard[ sw::input::Keyboard::KEY_DOWN ].IsKeyDownEvent() )
		{
			if( m_farClip - moveSpeed > m_nearClip )
				m_farClip -= moveSpeed;
		}
	}
	else
	{
		if( keyboard[ sw::input::Keyboard::KEY_UP ].IsKeyDownEvent() )
		{
			m_nearClip += moveSpeed;
		}

		if( keyboard[ sw::input::Keyboard::KEY_DOWN ].IsKeyDownEvent() )
		{
			if( m_nearClip + moveSpeed < m_farClip )
				m_nearClip -= moveSpeed;
		}
	}

}


}