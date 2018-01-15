#pragma once

#include "swGraphicAPI/Resources/MeshResources.h"
#include "ShellMeshTools/ShellMesh/ShellMeshVertex.h"
#include "ShellMeshTools/ShellMesh/TexturedVertex.h"
#include "ShellMeshTools/Animation/Animation.h"

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
struct Material
{
	std::string			TexturePath;

// ================================ //
//
	Material( const std::string& texPath )
		: TexturePath( texPath )
	{}
};


// ================================ //
//
template< typename VertexType >
struct TemporaryMeshInit
{
	std::vector< VertexType >				Verticies;
	std::vector< std::vector< Index32 > >	Indicies;
};

typedef TemporaryMeshInit< vr::ShellMeshVertex > TempShellMeshInit;

// ================================ //
//
struct TexturedMesh
{
	std::vector< vr::TexturedVertex >		Verticies;
	std::vector< std::vector< Index32 > >	Indicies;
	std::vector< Material >					Materials;
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

