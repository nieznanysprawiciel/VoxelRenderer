#pragma once
/**@file FBXloader.h
@author nieznanysprawiciel
@copyright File based on FBXLoader from SWEngine.*/

#include "fbxsdk.h"
#include "swCommonLib/Common/TypesDefinitions.h"
#include "FbxHelperStructs.h"

#include "ShellMeshTools/ShellMesh/ShellMeshVertex.h"
#include "ShellMeshTools/ShellMesh/ShellMesh.h"
#include "ShellMeshTools/Animation/Animation.h"
#include "ShellMeshTools/Animation/Skeleton.h"

#include "swCommonLib/System/Path.h"
#include "swCommonLib/Common/Nullable.h"


namespace vr
{


struct FbxMeshCollection;
class Model3DFromFile;		///< @deprecated

/**@brief Klasa s³u¿y do wczytywania plików w formacie Autodesk FBX.


@ingroup MakingLoaders*/
class FBXLoader
{
private:
	FbxManager*			fbx_manager;
	FbxIOSettings*		fbx_IOsettings;

	filesystem::Path	m_filePath;

public:
	FBXLoader();
	~FBXLoader();


	Nullable< TexturedMesh >		LoadTexturedMesh	( const filesystem::Path& fileName );
	Nullable< vr::ShellMeshPtr >	LoadMesh			( ResourceManager* manager, const filesystem::Path& fileName );
	bool							CanLoad				( const filesystem::Path& fileName );

private:

	vr::SkeletonPtr					LoadSkeleton	( FbxNode* rootNode );
	void							BuildSkeleton	( std::vector< vr::Joint >& joints, FbxNode* node, int parentIdx );

	vr::AnimationPtr				LoadAnimation	( Nullable< FbxMeshCollection >& nodes, FbxScene* scene, SkeletonPtr skeleton );
	void							LoadAnimation	( FbxNode* node, FbxScene* scene, TemporaryAnimationInit & animInit, SkeletonPtr skeleton );

	Nullable< FbxMeshCollection >	ProcessNode		( FbxNode* node, Nullable< FbxMeshCollection >& meshes );
	Nullable< TempShellMeshInit >	ProcessMesh		( FbxNodeMesh& nodeData, Nullable< TempShellMeshInit >& mesh, SkeletonPtr skeleton );
	Nullable< TexturedMesh >		ProcessMesh		( FbxScene* scene, FbxNodeMesh& nodeData, Nullable< TexturedMesh >& mesh );

	std::vector< Material >			ListMaterials	( FbxScene* scene );
	std::vector< uint32 >			MaterialMap		( FbxScene* scene, FbxNode* node );

	void							Scale			( Nullable< TempShellMeshInit >& mesh );
	void							RepairWeights	( Nullable< TempShellMeshInit >& mesh );

	template< typename VertexType >
	void							TransformVerticies	( std::vector< VertexType >& verticies, uint32 offset, const DirectX::XMFLOAT4X4& matrix );


private:

	FbxScene*						LoadFbxScene		( const filesystem::Path& fileName );

	static DirectX::XMFLOAT3		GetVertexNormal		( FbxMesh* mesh, uint32 polygonCounter, uint32 vertexIdx );
	static DirectX::XMFLOAT2		GetVertexUV			( FbxMesh* mesh, uint32 polygonCounter, uint32 vertexIdx, const char* setName );

	int								ReadMaterialIndex	( FbxMesh* mesh, unsigned int polygonCounter );
};


}	// sw


