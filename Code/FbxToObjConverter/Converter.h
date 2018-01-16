#pragma once

#include "swCommonLib/System/Path.h"

#include "ShellMeshTools/Loader/FbxHelperStructs.h"


namespace vr
{



// ================================ //
//
class Converter
{
public:

	static bool				Convert		( const filesystem::Path& inputFile, const filesystem::Path& outputFile, bool genMatfile );
	
private:

	static filesystem::Path	DefaultMatListFile	( const filesystem::Path& outputFile );
	static bool				SaveMaterialsList	( const filesystem::Path& outputFile, const std::vector< Material >& materials );
};



}


