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

	bool			m_flipU;
	bool			m_flipV;

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

	bool			SetSampler			( const std::string& samplerName );
	void			SetSampler			( SamplerType type ) { m_samplerType = type; }
	void			FlipUV				( bool u, bool v ) { m_flipU = u; m_flipV = v; }
};




