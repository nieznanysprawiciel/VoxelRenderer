#pragma once

#include "ShellMeshTools/Animation/Skeleton.h"
#include "ShellMeshTools/Animation/Animation.h"

#include "swGraphicAPI/Resources/MeshResources.h"
#include "swGraphicAPI/ResourceManager/ResourceManager.h"

#include "VoxelRenderer/SVO/Octree.h"

#include "ShellMeshTools/Loader/FbxHelperStructs.h"


namespace vr
{


/**@brief */
class ShellMesh
{
private:

	// =========================
	SkeletonPtr			m_skeleton;
	AnimationPtr		m_animation;		///< Support only one animation at the moment.

	ResourcePtr< BufferObject >		m_shellMesh;			///< VertexBuffer
	ResourcePtr< BufferObject >		m_shellMeshIndex;		///< IndexBuffer
	ResourcePtr< BufferObject >		m_animMatricies;

	Size				m_numVerticies;
	Size				m_numIndiecies;

	DirectX::XMFLOAT3	m_translate;
	float				m_scale;

	// =========================
	OctreePtr						m_octree;
	
	ResourcePtr< BufferObject >		m_octreeBuffer;
	ResourcePtr< TextureObject >	m_octreeTexBuff;

protected:
public:
	
	explicit		ShellMesh		( ResourceManager* manager, SkeletonPtr skeleton, AnimationPtr anim, TemporaryMeshInit& meshInitData );
	~ShellMesh	() = default;

	BufferObject*			GetVertexBuffer	()		{ return m_shellMesh.Ptr(); }
	BufferObject*			GetIndexBuffer	()		{ return m_shellMeshIndex.Ptr(); }

	BufferObject*			GetOctreeBuffer	()		{ return m_octreeBuffer.Ptr(); }
	TextureObject*			GetOctreeTexture()		{ return m_octreeTexBuff.Ptr(); }
	OctreePtr				GetOctree		()		{ return m_octree; }

	Size					GetNumVerticies	() const { return m_numVerticies; }
	Size					GetNumIndicies	() const { return m_numIndiecies; }

	AnimationPtr			GetAnimation	() const { return m_animation; }

	DirectX::XMFLOAT3		GetTranslate	() const { return m_translate; }
	float					GetScale		() const { return m_scale; }

	void					ApplyOctree		( ResourceManager* manager, OctreePtr octree );

	std::vector< Transform >			Evaluate			( TimeType time );

private:

	void								ComputeScale		( TemporaryMeshInit& meshInitData );
	std::vector< ShellMeshVertex >		BuildVertexBuffer	( TemporaryMeshInit& meshInitData );
	void								SortTriangleWeights	( ShellMeshVertex& point1, ShellMeshVertex& point2, ShellMeshVertex& point3 );

	void								AddWeights			( std::set< uint8 >& idxSet, ShellMeshVertex& point );
	void								ApplyScaleToAnim	( std::vector< Transform >& bonesTransforms );

public:

	static InputLayoutDescriptor*		CreateShellLayout	();
};

DEFINE_PTR_TYPE( ShellMesh );

}

