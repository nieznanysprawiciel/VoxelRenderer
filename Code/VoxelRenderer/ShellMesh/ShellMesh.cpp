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
}

}


