#pragma once
/**@file FBXloader.h
@author nieznanysprawiciel
@copyright File based on FBXLoader from SWEngine.*/

#include "fbxsdk.h"
#include "FbxHelperStructs.h"

#include "ShellMeshVertex.h"
#include "ShellMesh.h"
#include "VoxelRenderer/Animation/Animation.h"
#include "VoxelRenderer/Animation/Skeleton.h"

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

	Nullable< vr::ShellMeshPtr >	LoadMesh	( ResourceManager* manager, const filesystem::Path& fileName );
	bool							CanLoad		( const filesystem::Path& fileName );

private:

	vr::SkeletonPtr					LoadSkeleton	( FbxNode* rootNode );
	void							BuildSkeleton	( std::vector< vr::Joint >& joints, FbxNode* node, int parentIdx );

	vr::AnimationPtr				LoadAnimation	( Nullable< FbxMeshCollection >& nodes, FbxScene* scene, SkeletonPtr skeleton );
	void							LoadAnimation	( FbxNode* node, FbxScene* scene, TemporaryAnimationInit & animInit, SkeletonPtr skeleton );

	Nullable< FbxMeshCollection >	ProcessNode		( FbxNode* node, Nullable< FbxMeshCollection >& meshes );
	Nullable< TemporaryMeshInit >	ProcessMesh		( FbxNodeMesh& nodeData, Nullable< TemporaryMeshInit >& mesh, SkeletonPtr skeleton );

	void							Scale			( Nullable< TemporaryMeshInit >& mesh );

	void		TransformVerticies	( std::vector< vr::ShellMeshVertex >& verticies, uint32 offset, const DirectX::XMFLOAT4X4& matrix );
};


}	// sw


