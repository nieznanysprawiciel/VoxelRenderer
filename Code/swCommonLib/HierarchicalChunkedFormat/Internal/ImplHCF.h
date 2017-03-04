#pragma once
/**
@file ImplHCF.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "ChunkRepr.h"
#include "AttributeRepr.h"

#include "swCommonLib/HierarchicalChunkedFormat/Chunk.h"
#include "swCommonLib/HierarchicalChunkedFormat/Attribute.h"

#include "swCommonLib/HierarchicalChunkedFormat/Headers/FileHeader.h"

#include "swCommonLib/System/Path.h"


#include <stdio.h>



namespace sw
{

/**@brief */
class ImplHCF
{
private:

	FileHeader			m_header;

	ChunkReprPtr		m_rootChunk;			///< Main Chunk.
	AttributeReprPtr	m_fileAttributes;		///< Top level attributes.

	// File related things.
	FILE*				m_file;
	Size				m_writePtr;

public:

	bool				m_directWrite			: 1;
	bool				m_attributesWritten		: 1;
	bool				m_fileReadOnly			: 1;

protected:
public:
	explicit		ImplHCF		();
					~ImplHCF	();


	Chunk			GetRootChunk	();
	Chunk			CreateRootChunk	();

	bool			OpenFile		( const filesystem::Path& filePath, bool writeDirect );
	bool			WriteFile		( const filesystem::Path& filePath );
	bool			LoadFile		( const filesystem::Path& filePath, bool readOnly );

	Attribute		AddGlobalAttribute	( AttributeType type, const DataPtr data, Size dataSize );
	Attribute		AddAttribute		( AttributeReprPtr& list, AttributeType type, const DataPtr data, Size dataSize );

public:
	// Internal implementation
	Size			ComputeWholeSize	( Attribute attrib );

	Size			ReserveMemory		( Size dataSize );
	FILE*			GetFile				()					{ return m_file; }

	bool			CanAddGlobalAttrib	() const;
};


}	// sw
