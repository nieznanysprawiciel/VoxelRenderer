#pragma once

#include "VoxelRenderer/SVO/Octree.h"
#include "VoxelConverter/Texturing/Samplers/Samplers.h"

#include "swCommonLib/System/Path.h"


namespace vr
{
	class OctreeBuilder;
}


/**@brief */
class VoxelConverter
{
	typedef std::vector< filesystem::Path > PathsVec;
private:

	PathsVec				m_texturesPaths;
	SamplerType				m_samplerType;

	bool			m_flipU;
	bool			m_flipV;

	bool			m_applyTextureToAll;

protected:
public:
	explicit		VoxelConverter		();
					~VoxelConverter		() = default;

	vr::OctreePtr	Load				( const filesystem::Path& inputFilePath );
	bool			Write				( const filesystem::Path& outputFilePath, vr::OctreePtr octree );

	bool			Convert				( const filesystem::Path& inputFilePath, const filesystem::Path& outputFilePath );

	/**@brief Adds texture on the end of texture vector.*/
	void			AddTexture			( const filesystem::Path& texPath );

	/**@brief Applies first texture to all voxels.
	@todo Implement.*/
	void			ApplyTextureToAll	() { m_applyTextureToAll = true; }

	/**@brief Loads texture information from .matlist file.
	This is best way to texture model with multiple textures.*/
	void			LoadMatListFile		( const filesystem::Path& texPath );

	bool			SetSampler			( const std::string& samplerName );
	void			SetSampler			( SamplerType type ) { m_samplerType = type; }
	void			FlipUV				( bool u, bool v ) { m_flipU = u; m_flipV = v; }

private:

	void			ApplyTextures		( vr::OctreeBuilder& builder );
};




