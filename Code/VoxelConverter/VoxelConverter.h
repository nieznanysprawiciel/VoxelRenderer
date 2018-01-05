#pragma once

#include "VoxelRenderer/SVO/Octree.h"
#include "VoxelConverter/Texturing/Samplers/Samplers.h"

#include "swCommonLib/System/Path.h"


/**@brief */
class VoxelConverter
{
private:

	filesystem::Path		m_texturePath;
	SamplerType				m_samplerType;

protected:
public:
	explicit		VoxelConverter		();
					~VoxelConverter		() = default;

	vr::OctreePtr	Load				( const filesystem::Path& inputFilePath );
	bool			Write				( const filesystem::Path& outputFilePath, vr::OctreePtr octree );

	bool			Convert				( const filesystem::Path& inputFilePath, const filesystem::Path& outputFilePath );

	/**@brief Adds texture. Overwrites previous if existed.
	@todo In future add support for multiple textures. To do this we must encode
	texture index for every voxel.*/
	void			AddTexture			( const filesystem::Path& texPath );

	void			SetSampler			( SamplerType type ) { m_samplerType = type; }
};




