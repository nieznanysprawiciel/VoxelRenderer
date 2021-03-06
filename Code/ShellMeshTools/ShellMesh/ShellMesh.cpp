#include "ShellMeshTools/stdafx.h"
#include "ShellMesh.h"

#include "swGraphicAPI/Resources/ResourcesFactory.h"


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
ShellMesh::ShellMesh		( ResourceManager* manager, SkeletonPtr skeleton, AnimationPtr anim, TempShellMeshInit& meshInitData )
	:	m_skeleton( skeleton )
	,	m_animation( anim )
{
	m_numVerticies = meshInitData.Verticies.size();
	m_numIndiecies = 0;

	for( Size i = 0; i < meshInitData.Indicies.size(); ++i )
	{
		m_numIndiecies += meshInitData.Indicies[ i ].size();
	}

	ComputeScale( meshInitData );

	MemoryChunk indexChunk( (uint32)m_numIndiecies * sizeof( Index32 ) );
	CopyIndexBuffer< Index32 >( meshInitData.Indicies, indexChunk );

	m_shellMesh = manager->CreateVertexBuffer( L"::ShellMeshVertex", (uint8*)meshInitData.Verticies.data(), sizeof( ShellMeshVertex ), (uint32)meshInitData.Verticies.size() );
	m_shellMeshIndex = manager->CreateIndexBuffer( L"ShellMeshIndex", indexChunk.GetMemory< uint8* >(), sizeof( Index32 ), (uint32)m_numIndiecies );

	// Only check now
	//BuildVertexBuffer( meshInitData );
}

// ================================ //
//
void			ShellMesh::ComputeScale		( TempShellMeshInit & meshInitData )
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
		if( vertex.Position.z < min.z ) min.z = vertex.Position.z;

		if( vertex.Position.x > max.x ) max.x = vertex.Position.x;
		if( vertex.Position.y > max.y ) max.y = vertex.Position.y;
		if( vertex.Position.z > max.z ) max.z = vertex.Position.z;
	}

	DirectX::XMFLOAT3 range( max.x - min.x, max.y - min.y, max.z - min.z );
	DirectX::XMFLOAT3 center( ( max.x + min.x ) / 2.0f, ( max.y + min.y ) / 2.0f, ( max.z + min.z ) / 2.0f );
	float rangeMax = std::max( std::max( range.x, range.y ), range.z );

	m_translate = DirectX::XMFLOAT3( -center.x, -center.y, -center.z );
	m_scale = 1.0f / rangeMax;
}

// ================================ //
// Split verticies. Each vertex will be referenced only once.
// Then sort triangles weights.
std::vector< ShellMeshVertex >		ShellMesh::BuildVertexBuffer	( TempShellMeshInit& meshInitData )
{
	std::vector< ShellMeshVertex > vertexBuffer;

	auto & verticies = meshInitData.Verticies;

	for( auto & indexBuff : meshInitData.Indicies )
	{
		for( auto idx : indexBuff )
			vertexBuffer.push_back( verticies[ idx ] );
	}

	for( Size i = 0; i < vertexBuffer.size(); i += 3 )
		SortTriangleWeights( vertexBuffer[ i ], vertexBuffer[ i + 1 ], vertexBuffer[ i + 2 ] );

	return vertexBuffer;
}

// ================================ //
// Each triangle should have only 4 weight indicies which it references.
void								ShellMesh::SortTriangleWeights	( ShellMeshVertex& point1,
																	  ShellMeshVertex& point2,
																	  ShellMeshVertex& point3 )
{
	std::set< uint8 > uniqueIdx;

	AddWeights( uniqueIdx, point1 );
	AddWeights( uniqueIdx, point2 );
	AddWeights( uniqueIdx, point3 );

	if( uniqueIdx.size() > 4 )
	{
		static Size numOverflows = 0;
		numOverflows++;
	}

}

// ================================ //
//
void								ShellMesh::AddWeights			( std::set< uint8 >& idxSet, ShellMeshVertex& point )
{
	for( int i = 0; i < 4; ++i )
	{
		if( point.Weights[ i ] > 0.0f )
			idxSet.insert( point.WeightIdx[ i ] );
	}
}

// ================================ //
//
void								ShellMesh::ApplyScaleToAnim		( std::vector< Transform >& bonesTransforms )
{
	DirectX::XMMATRIX scaling = DirectX::XMMatrixScaling( m_scale, m_scale, m_scale );
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation ( m_translate.x, m_translate.y, m_translate.z );

	DirectX::XMMATRIX postTransform = DirectX::XMMatrixMultiply( translate, scaling );

	// Apply scaling and translation to global bind pose inverse matrix.
	for( auto & transform : bonesTransforms )
	{
		DirectX::XMMATRIX boneMatrix = DirectX::XMLoadFloat4x4( &transform );
		boneMatrix = DirectX::XMMatrixTranspose( boneMatrix );
		boneMatrix = DirectX::XMMatrixMultiply( boneMatrix, postTransform );
		boneMatrix = DirectX::XMMatrixTranspose( boneMatrix );

		DirectX::XMStoreFloat4x4( &transform, boneMatrix );
	}
}

// ================================ //
//
InputLayoutDescriptor*				ShellMesh::CreateShellLayout	()
{
	auto animLayoutDesc = ResourcesFactory::CreateInputLayoutDescriptor( L"::AnimationLayout" );
	animLayoutDesc->AddRow( "POSITION", ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 0, false, 0 );
	animLayoutDesc->AddRow( "BLENDINDICES", ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_UINT, 0, 12, false, 0 );
	animLayoutDesc->AddRow( "BLENDWEIGHT", ResourceFormat::RESOURCE_FORMAT_R32G32B32A32_FLOAT, 0, 16, false, 0 );
	animLayoutDesc->AddRow( "NORMAL", ResourceFormat::RESOURCE_FORMAT_R32G32B32_FLOAT, 0, 32, false, 0 );

	return animLayoutDesc;
}

// ================================ //
//
void								ShellMesh::ApplyOctree			( ResourceManager* manager, OctreePtr octree )
{
	if( m_octree != octree )
	{
		m_octree = octree;

		TextureBufferInitData bufferDesc;
		bufferDesc.AllowRaw = true;
		bufferDesc.ElementSize = sizeof( OctreeNode );
		bufferDesc.NumElements = (uint32)octree->AccessOctree().size();
		bufferDesc.Usage = ResourceUsage::RESOURCE_USAGE_DEFAULT;
		bufferDesc.Data = (uint8*)octree->AccessOctree().data();
		
		// We should release old octree here.

		// @todo: we should give unique names to buffers otherwise, we can't create more then one shell mesh octree.
		m_octreeBuffer = manager->CreateTextureBuffer( L"DynamicOctree", bufferDesc );
		m_octreeTexBuff = m_octreeBuffer->CreateRawShaderView( L"RawDynamicOctreeView", manager );
	}
}

// ================================ //
//
std::vector< Transform >			ShellMesh::Evaluate				( TimeType time )
{
	auto bonesTransforms = m_animation->Evaluate( time );
	
	ApplyScaleToAnim( bonesTransforms );

	return bonesTransforms;
}

}


