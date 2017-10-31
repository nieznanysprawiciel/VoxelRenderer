#pragma once

#include "VoxelRenderer/Animation/Skeleton.h"
#include "VoxelRenderer/Animation/Animation.h"

#include "swGraphicAPI/Resources/MeshResources.h"
#include "swGraphicAPI/ResourceManager/ResourceManager.h"

#include "VoxelRenderer/ShellMesh/FbxHelperStructs.h"


namespace vr
{


/**@brief */
class ShellMesh
{
private:

	SkeletonPtr			m_skeleton;
	AnimationPtr		m_animation;		///< Support only one animation at the moment.


	ResourcePtr< BufferObject >		m_shellMesh;			///< VertexBuffer
	ResourcePtr< BufferObject >		m_shellMeshIndex;		///< IndexBuffer
	ResourcePtr< BufferObject >		m_animMatricies;

	Size				m_numVerticies;
	Size				m_numIndiecies;

	DirectX::XMFLOAT3	m_translate;
	float				m_scale;

protected:
public:
	explicit		ShellMesh		( ResourceManager* manager, SkeletonPtr skeleton, AnimationPtr anim, TemporaryMeshInit& meshInitData );
	~ShellMesh	() = default;

	BufferObject*			GetVertexBuffer	()		{ return m_shellMesh.Ptr(); }
	BufferObject*			GetIndexBuffer	()		{ return m_shellMeshIndex.Ptr(); }

	Size					GetNumVerticies	() const { return m_numVerticies; }
	Size					GetNumIndicies	() const { return m_numIndiecies; }

	AnimationPtr			GetAnimation	() const { return m_animation; }

	DirectX::XMFLOAT3		GetTranslate	() const { return m_translate; }
	float					GetScale		() const { return m_scale; }

private:

	void								ComputeScale		( TemporaryMeshInit& meshInitData );
	std::vector< ShellMeshVertex >		BuildVertexBuffer	( TemporaryMeshInit& meshInitData );
	void								SortTriangleWeights	( ShellMeshVertex& point1, ShellMeshVertex& point2, ShellMeshVertex& point3 );

	void								AddWeights			( std::set< uint8 >& idxSet, ShellMeshVertex& point );
};

DEFINE_PTR_TYPE( ShellMesh );

}

