#include "ShellMesh.h"




namespace vr
{


// ================================ //
//
template< typename IndexType >
inline void			CopyIndexBuffer		( const std::vector< std::vector< Index32 > >& indicies, MemoryChunk& destination )
{
	auto dest = destination.GetMemory< IndexType >();

	for( int i = 0; i < indicies.size(); ++i )
	{
		for( int indexIdx = 0; indexIdx < indicies[ i ].size(); indexIdx++ )
		{
			*dest = (IndexType)indicies[ i ][ indexIdx ];
			dest++;
		}
	}
}


// ================================ //
//
ShellMesh::ShellMesh		( ResourceManager* manager, SkeletonPtr skeleton, AnimationPtr anim, TemporaryMeshInit& meshInitData )
	:	m_skeleton( skeleton )
	,	m_animation( anim )
{
	m_numVerticies = meshInitData.Verticies.size();
	m_numIndiecies = 0;

	for( Size i = 0; i < meshInitData.Indicies.size(); ++i )
	{
		m_numIndiecies += meshInitData.Indicies[ i ].size();
	}

	MemoryChunk indexChunk( (uint32)m_numIndiecies * sizeof( Index32 ) );
	CopyIndexBuffer< Index32 >( meshInitData.Indicies, indexChunk );

	m_shellMesh = manager->CreateVertexBuffer( L"::ShellMeshVertex", (uint8*)meshInitData.Verticies.data(), sizeof( ShellMeshVertex ), (uint32)meshInitData.Verticies.size() );
	m_shellMeshIndex = manager->CreateIndexBuffer( L"ShellMeshIndex", indexChunk.GetMemory< uint8* >(), sizeof( Index32 ), (uint32)m_numIndiecies );

	ComputeScale( meshInitData );
}

// ================================ //
//
void			ShellMesh::ComputeScale		( TemporaryMeshInit & meshInitData )
{
	auto & verticies = meshInitData.Verticies;

	const float floatMax = std::numeric_limits< float >::max();
	const float floatMin = std::numeric_limits< float >::min();

	DirectX::XMFLOAT3 min( floatMax, floatMax, floatMax );
	DirectX::XMFLOAT3 max( floatMin, floatMin, floatMin );

	for( auto& vertex : verticies )
	{
		if( vertex.Position.x < min.x ) min.x = vertex.Position.x;
		if( vertex.Position.y < min.y ) min.y = vertex.Position.y;
		if( vertex.Position.z < min.z ) min.z = vertex.Position.x;

		if( vertex.Position.x > max.x ) max.x = vertex.Position.x;
		if( vertex.Position.y > max.y ) max.y = vertex.Position.y;
		if( vertex.Position.z > max.z ) max.z = vertex.Position.x;
	}

	DirectX::XMFLOAT3 range( max.x - min.x, max.y - min.y, max.z - min.z );
	DirectX::XMFLOAT3 center( ( max.x + min.x ) / 2.0f, ( max.y + min.y ) / 2.0f, ( max.z + min.z ) / 2.0f );
	float rangeMax = std::max( std::max( range.x, range.y ), range.z );

	m_translate = DirectX::XMFLOAT3( -center.x, -center.y, -center.z );
	m_scale = 1.0f / rangeMax;
}

}


