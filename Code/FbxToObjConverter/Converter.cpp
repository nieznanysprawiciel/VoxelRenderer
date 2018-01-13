#include "Converter.h"


#include "ShellMeshTools/Loader/FBXLoader.h"
#include "ObjExporter.h"


namespace vr
{




// ================================ //
//
bool			Converter::Convert			( const filesystem::Path& inputFile, const filesystem::Path& outputFile )
{
	FBXLoader loader;

	auto mesh = loader.LoadTexturedMesh( inputFile );
	if( mesh.IsValid )
	{
		ObjExporter exporter( mesh.Value );
		return exporter.Export( outputFile );
	}

	return false;
}

}	// vr


