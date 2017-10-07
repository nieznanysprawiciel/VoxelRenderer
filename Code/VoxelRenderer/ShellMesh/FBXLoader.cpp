#include "FBXLoader.h"
#include "FbxLoader.inl"

//#include "Tools/Geometric/Processors/Converter.h"



#include "swCommonLib/Common/Converters.h"
#include "swCommonLib/Common/MemoryLeaks.h"


#include <limits>


using namespace DirectX;

namespace vr
{


// ================================ //
// Helpers declarations

DirectX::XMFLOAT3	Get			( const fbxsdk::FbxVector4& vector );
bool				operator==	( const fbxsdk::FbxVector4& vec1, const DirectX::XMFLOAT3& vec2 );

void				AddWeight					( vr::ShellMeshVertex & vertex, float weight, uint8 idx );
unsigned int		FindJointIndexUsingName		( const std::string& name, SkeletonPtr skeleton );


/**@brief Suported file extensions which can be load by loader.*/
const char*	SupportedExtensions[] =
{
	".FBX",
	".OBJ",
	".3DS",
	".DXF"
};


///@brief Kontruktr inicjuje obiekty FBX SDK, konieczne do wczytania modelu.
FBXLoader::FBXLoader()
{
	fbx_manager = FbxManager::Create();
	fbx_IOsettings = FbxIOSettings::Create( fbx_manager, IOSROOT );
	fbx_manager->SetIOSettings( fbx_IOsettings );
}


FBXLoader::~FBXLoader()
{
	fbx_manager->Destroy();
	//nie zwalniamy fbx_IOsettings, bo zajmuje siê tym fbx_manager
}


//====================================================================================//
//			New loading	
//====================================================================================//

/**@brief Loads file.*/
Nullable< vr::ShellMeshPtr >		FBXLoader::LoadMesh		( ResourceManager* manager, const filesystem::Path& fileName )
{
	FbxImporter* FbxImporter = FbxImporter::Create( fbx_manager, "" );
	m_filePath = fileName;

	if( !FbxImporter->Initialize( m_filePath.String().c_str(), -1, fbx_manager->GetIOSettings() ) )
	{
		auto& status = FbxImporter->GetStatus();
		throw std::runtime_error( status.GetErrorString() );
	}

	FbxScene* scene = FbxScene::Create( fbx_manager, "Scene" );
	FbxImporter->Import( scene );
	FbxImporter->Destroy();

	// Triangulate scene before loading.
	FbxGeometryConverter triangulator( fbx_manager );
	triangulator.Triangulate( scene, true );

	FbxNode* root = scene->GetRootNode();
	if( root == nullptr )
		return "Root node is nullptr";

	auto skeleton = LoadSkeleton( root );

	Nullable< FbxMeshCollection > meshData( NullableInit::Valid );
	for( int i = 0; i < scene->GetNodeCount(); i++ )
		meshData = ProcessNode( scene->GetNode( i ), meshData );

	ReturnIfInvalid( meshData );

	Nullable< TemporaryMeshInit > tempMeshInit( NullableInit::Valid );

	for( auto& mesh : meshData.Value.Segments )
	{
		tempMeshInit = ProcessMesh( mesh, tempMeshInit, skeleton );
	}

	Scale( tempMeshInit );


	ReturnIfInvalid( tempMeshInit );

	return std::make_shared< vr::ShellMesh >( manager, skeleton, nullptr, tempMeshInit.Value );
}

/**@brief Returns true if loader can load specific file.

This function uses file extension to determine if it can load file.*/
bool						FBXLoader::CanLoad		( const filesystem::Path& fileName )
{
	std::string extension = fileName.GetExtension();
	std::transform( extension.begin(), extension.end(), extension.begin(), toupper );

	for( auto ext : SupportedExtensions )
	{
		if( extension == ext )
			return true;
	}
	return false;
}


/**@brief Process single node.*/
Nullable< FbxMeshCollection >		FBXLoader::ProcessNode		( FbxNode* node, Nullable< FbxMeshCollection >& meshes )
{
	ReturnIfInvalid( meshes );

	if( node == nullptr )
		return std::move( meshes );

	// Compute transform matrix for this node.
	FbxAMatrix transformMatrix = node->EvaluateGlobalTransform();
	DirectX::XMFLOAT4X4 transformation;
	for( int i = 0; i < 4; ++i )
	{
		for( int j = 0; j < 4; ++j )
			transformation( i, j ) = static_cast<float>( transformMatrix.Get( i, j ) );
	}

	for( int i = 0; i < node->GetNodeAttributeCount(); i++ )
	{
		if( ( node->GetNodeAttributeByIndex( i ) )->GetAttributeType() == FbxNodeAttribute::eMesh )
		{
			// Look only for meshes. Ignore rest
			FbxNodeMesh mesh;
			mesh.Transformation = transformation;
			mesh.Node = node;
			mesh.Mesh = (FbxMesh*)node->GetNodeAttributeByIndex( i );

			meshes.Value.Segments.push_back( mesh );
		}
	}

	//for( int i = 0; i < node->GetChildCount(); i++ )
	//	meshes = ProcessNode( node->GetChild( i ), meshes );

	return std::move( meshes );
}


/**@brief Process single mesh.*/
Nullable< TemporaryMeshInit >		FBXLoader::ProcessMesh		( FbxNodeMesh& nodeData, Nullable< TemporaryMeshInit >& mesh, SkeletonPtr skeleton )
{
	ReturnIfInvalid( mesh );

	FbxNode* fbxNode = nodeData.Node;
	FbxMesh* fbxMesh = nodeData.Mesh;
	uint32 ctrlPointsOffset = (uint32)mesh.Value.Verticies.size();

	// ================================ //
	// Build index buffer
	unsigned int polygonsCount = fbxMesh->GetPolygonCount();

	std::vector< Index32 > indicies;		indicies.reserve( 3 * polygonsCount );

	for( Size i = 0; i < polygonsCount; ++i )
	{
		for( int vertexIdx = 0; vertexIdx < 3; ++vertexIdx )
		{
			Index32 controlPointIdx = (Index32)fbxMesh->GetPolygonVertex( (int)i, vertexIdx );
			indicies.push_back( controlPointIdx + ctrlPointsOffset );
		}
	}

	// ================================ //
	// Build vertex buffer
	auto fbxControlPoints = fbxMesh->GetControlPoints();
	std::vector< vr::ShellMeshVertex > controlPoints;		controlPoints.resize( fbxMesh->GetControlPointsCount() );

	// Create control points array with weights.
	for( Size i = 0; i < fbxMesh->GetControlPointsCount(); ++i )
	{
		vr::ShellMeshVertex& curVertex = controlPoints[ i ];

		curVertex.Position = Get( fbxControlPoints[ i ] );
		curVertex.Weights[ 0 ] = 0.0f;
		curVertex.Weights[ 1 ] = 0.0f;
		curVertex.Weights[ 2 ] = 0.0f;
		curVertex.Weights[ 3 ] = 0.0f;
	}

	// Process deformers to extract weights.
	unsigned int numDeformers = fbxMesh->GetDeformerCount();
	for( unsigned int deformerIndex = 0; deformerIndex < numDeformers; ++deformerIndex )
	{
		FbxSkin* currSkin = static_cast< FbxSkin* >( fbxMesh->GetDeformer( deformerIndex, FbxDeformer::eSkin ) );
		if( currSkin->GetDeformerType() == FbxDeformer::eSkin )
		{
			unsigned int numOfClusters = currSkin->GetClusterCount();
			for( unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex )
			{
				FbxCluster* currCluster = currSkin->GetCluster( clusterIndex );
				std::string currJointName = currCluster->GetLink()->GetName();
				unsigned int currJointIndex = FindJointIndexUsingName( currJointName, skeleton );

				unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
				for( unsigned int i = 0; i < numOfIndices; ++i )
				{
					auto & curControlPoint = controlPoints[ currCluster->GetControlPointIndices()[ i ] ];
					AddWeight( curControlPoint, (float)currCluster->GetControlPointWeights()[ i ], currJointIndex );
				}
			}
		}
	}

	// ================================ //
	// Copy prepared vertex buffer.
	for( auto& shellVertex : controlPoints )
	{
		mesh.Value.Verticies.push_back( shellVertex );
	}

	// Verticies in FBX file have transformations which we must apply.
	// We don't have to preserve transformation matrix for each segment with this approach.
	TransformVerticies( mesh.Value.Verticies, ctrlPointsOffset, nodeData.Transformation );

	mesh.Value.Indicies.push_back( std::move( indicies ) );

	return std::move( mesh );
}

// ================================ //
//
void								FBXLoader::Scale					( Nullable< TemporaryMeshInit >& mesh )
{
	if( mesh.IsValid )
	{
		auto & verticies = mesh.Value.Verticies;

		const float floatMax = std::numeric_limits< float >::max();
		const float floatMin = std::numeric_limits< float >::min();

		XMFLOAT3 min( floatMax, floatMax, floatMax );
		XMFLOAT3 max( floatMin, floatMin, floatMin );

		for( auto& vertex : verticies )
		{
			if( vertex.Position.x < min.x ) min.x = vertex.Position.x;
			if( vertex.Position.y < min.y ) min.y = vertex.Position.y;
			if( vertex.Position.z < min.z ) min.z = vertex.Position.x;

			if( vertex.Position.x > max.x ) max.x = vertex.Position.x;
			if( vertex.Position.y > max.y ) max.y = vertex.Position.y;
			if( vertex.Position.z > max.z ) max.z = vertex.Position.x;
		}

		XMFLOAT3 range( max.x - min.x, max.y - min.y, max.z - min.z );
		float rangeMax = std::max( std::max( range.x, range.y ), range.z );

		for( auto& vertex : verticies )
		{
			vertex.Position.x = vertex.Position.x / rangeMax;
			vertex.Position.y = vertex.Position.y / rangeMax;
			vertex.Position.z = vertex.Position.z / rangeMax;
		}
	}
}



// ================================ //
//
vr::SkeletonPtr						FBXLoader::LoadSkeleton				( FbxNode* rootNode )
{
	std::vector< vr::Joint > joints;

	for( int childIndex = 0; childIndex < rootNode->GetChildCount(); ++childIndex )
	{
		FbxNode* currNode = rootNode->GetChild( childIndex );
		BuildSkeleton( joints, currNode, -1 );
	}

	return std::make_shared< vr::Skeleton >( std::move( joints ) );
}

// ================================ //
//
void								FBXLoader::BuildSkeleton			( std::vector< vr::Joint >& joints, FbxNode* node, int parentIdx )
{
	int thisIdx = (int)joints.size() - 1;

	if( node->GetNodeAttribute() &&
		node->GetNodeAttribute()->GetAttributeType() &&
		node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton )
	{
		vr::Joint currJoint;
		currJoint.ParentIndex = parentIdx;
		currJoint.Name = node->GetName();
		joints.push_back( currJoint );
	}

	for( int i = 0; i < node->GetChildCount(); i++ )
	{
		BuildSkeleton( joints, node->GetChild( i ), thisIdx );
	}
}

// ================================ //
//
void								FBXLoader::TransformVerticies		( std::vector< vr::ShellMeshVertex >& verticies, uint32 offset, const DirectX::XMFLOAT4X4& matrix )
{
	XMMATRIX transform = XMLoadFloat4x4( &matrix );

	for( uint32 i = offset; i < verticies.size(); ++i )
	{
		auto& vertex = verticies[ i ];

		XMVECTOR position = XMLoadFloat3( &vertex.Position );
		position = XMVector3Transform( position, transform );
		XMStoreFloat3( &vertex.Position, position );
	}
}


//====================================================================================//
//			Helpers	
//====================================================================================//


// ================================ //
//
DirectX::XMFLOAT3	Get( const fbxsdk::FbxVector4& vector )
{
	DirectX::XMFLOAT3 result;
	result.x = (float)vector.mData[ 0 ];
	result.y = (float)vector.mData[ 1 ];
	result.z = (float)vector.mData[ 2 ];
	return result;
}

// ================================ //
// This function adds new weight. Weights are sorted by their importance. Since there's place
// for only 4 weights, the smallest will be removed.
void				AddWeight	( vr::ShellMeshVertex & vertex, float weight, uint8 idx )
{
	float addWeight = weight;
	uint8 addIdx = idx;

	for( int i = 0; i < 4; ++i )
	{
		if( vertex.Weights[ i ] < addWeight )
		{
			float tmpWeight = addWeight;
			uint8 tmpIdx = addIdx;

			addWeight = vertex.Weights[ i ];
			addIdx = vertex.WeightIdx[ i ];

			vertex.Weights[ i ] = tmpWeight;
			vertex.WeightIdx[ i ] = tmpIdx;
		}
	}
}

// ================================ //
//
unsigned int		FindJointIndexUsingName		( const std::string& name, SkeletonPtr skeleton )
{
	auto& joints = skeleton->GetJoints();
	for( int i = 0; i < joints.size(); ++i )
	{
		if( joints[ i ].Name == name )
			return i;
	}

	assert( false );
	return std::numeric_limits< unsigned int >::max();
}


}	// sw

