#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/Common/Version.h"

#include "swCommonLib/HierarchicalChunkedFormat/HCF.h"


namespace vr
{


/**@brief Header for .voxtree file format.*/
struct VoxtreeHeader
{
	char		VoxtreeLetters[ 7 ];		///< Letters "voxtree" can be used to check if file contains proper format.
	Version		VoxtreeVersion;				///< Version of this file format.
	Size		GridSize;
	Size		FreeIndirectOffset;			///< Offset to first free indirect pointer.

// ================================ //
//
	inline VoxtreeHeader()
		:	GridSize( 0 )
		,	FreeIndirectOffset( 0 )
	{
		const char* voxtree = "voxtree";
		memcpy( VoxtreeLetters, voxtree, 7 );
	}
};


} // vr



template<>
inline sw::AttributeType		GetAttributeTypeID< vr::VoxtreeHeader >		()
{
	return sw::HCF::UserDefinedAttributes + 1;
}

