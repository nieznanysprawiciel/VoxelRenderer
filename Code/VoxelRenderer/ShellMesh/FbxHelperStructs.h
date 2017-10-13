#pragma once

#include "swGraphicAPI/Resources/MeshResources.h"
#include "ShellMeshVertex.h"
#include "VoxelRenderer/Animation/Animation.h"

#include "fbxsdk.h"

#include <DirectXMath.h>
#include <vector>

namespace vr
{

// ================================ //
//
enum FbxShadingModel
{
	HardwareShader,
	Lambert,
	Phong
};


// ================================ //
//
struct FbxNodeMesh
{
	DirectX::XMFLOAT4X4		Transformation;
	FbxNode*				Node;
	FbxMesh*				Mesh;
};


// ================================ //
//
struct FbxMeshCollection
{
	std::vector< FbxNodeMesh >		Segments;
};

// ================================ //
//
struct TemporaryMeshInit
{
	std::vector< vr::ShellMeshVertex >		Verticies;
	std::vector< std::vector< Index32 > >	Indicies;
};

// ================================ //
//
struct TemporaryAnimationInit
{
	FbxTime		Start;
	FbxTime		End;

	std::vector< JointAnimation >	JointsAnims;
};

}	// vr

