#pragma once
/**
@file FileHeader.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/Common/Version.h"


namespace sw
{


/**@brief Hierarchical Chunked Format file header.

Main header.*/
struct FileHeader
{
	char			LetterShortcut[ 3 ];	///< Header.
	bool			LittleEndian;			///< Endiannes of hardware which have written this file.
	Size			FileSize;				///< Size including header.
	Size			RootChunkOffset;		///< First chunk in file.
	Version			VersionHCF;				///< Version of HCF file library.


// ================================ //
//
	/**@brief */
	FileHeader		( Version version )
		: VersionHCF( version )
	{
		LetterShortcut[ 0 ] = 'H';
		LetterShortcut[ 1 ] = 'C';
		LetterShortcut[ 2 ] = 'F';

		FileSize = 0;
		RootChunkOffset = 0;
		LittleEndian = CheckLittleEndian();
	}

	/**@brief
	Based on: http://stackoverflow.com/questions/4181951/how-to-check-whether-a-system-is-big-endian-or-little-endian
	*/
	bool	CheckLittleEndian()
	{
		int n = 1;
		// little endian if true
		if( *(char *)&n == 1 )
			return true;
		return false;
	}
};

}	// sw

