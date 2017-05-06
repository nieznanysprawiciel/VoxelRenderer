#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"

#include "swCommonLib/HierarchicalChunkedFormat/HCF.h"



namespace sw
{


class CLASS_TESTER( HCF )
{
private:
public:

	static bool			ValidateStreamPos		( sw::HCF* hcf, sw::Chunk& chunk );
	static bool			ValidateStreamPos		( sw::HCF* hcf, sw::Attribute& attrib );
};


// ================================ //
//
inline bool			CLASS_TESTER( HCF )::ValidateStreamPos		( sw::HCF* hcf, sw::Chunk& chunk )
{
	auto file = hcf->m_impl.GetFile();
	if( file == nullptr )
		return false;

	Size position = ftell( file );

	Size chunkOffset = chunk.m_chunkPtr->m_absolutOffset;
	Size dataOffset = chunk.m_chunkPtr->m_header.DataOffset;
	Size dataSize = chunk.m_chunkPtr->m_header.DataSize;

	if( position != chunkOffset + dataOffset + dataSize )
		return false;
	return true;
}

// ================================ //
//
inline bool			CLASS_TESTER( HCF )::ValidateStreamPos		( sw::HCF* hcf, sw::Attribute& attrib )
{
	auto file = hcf->m_impl.GetFile();
	if( file == nullptr )
		return false;

	Size position = ftell( file );

	Size chunkOffset = attrib.m_attribPtr->m_absolutOffset;
	Size attribSize = attrib.m_attribPtr->m_header.AttribSize;

	if( position != chunkOffset + attribSize )
		return false;
	return true;
}

}	// sw


