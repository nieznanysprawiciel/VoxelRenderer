#include "Converter.h"


#include "ShellMeshTools/Loader/FBXLoader.h"
#include "ObjExporter.h"

#include <fstream>


namespace vr
{




// ================================ //
//
bool			Converter::Convert			( const filesystem::Path& inputFile, const filesystem::Path& outputFile, bool genMatfile )
{
	FBXLoader loader;

	auto mesh = loader.LoadTexturedMesh( inputFile );
	if( mesh.IsValid )
	{
		ObjExporter exporter( mesh.Value );
		
		if( exporter.Export( outputFile ) )
		{
			if( !genMatfile )
				return true;

			/// @todo Paths in materials vector are relative to FBX file. We should translate them relative to
			/// outputFile path. This works as long as input and output are in the same folder.
			return SaveMaterialsList( DefaultMatListFile( outputFile ), mesh.Value.Materials );
		}
	}

	return false;
}

// ================================ //
//
filesystem::Path		Converter::DefaultMatListFile		( const filesystem::Path& outputFile )
{
	auto directory = outputFile.GetDirectory();
	auto baseName = outputFile.GetFileName();
	auto extension = std::string( ".matlist" );

	return directory / ( baseName + extension );
}

// ================================ //
//
bool					Converter::SaveMaterialsList		( const filesystem::Path& outputFile, const std::vector< Material >& materials )
{
	std::fstream file( outputFile.String(), std::fstream::out );

	if( file.is_open() )
	{
		for( auto & material : materials )
		{
			file << material.TexturePath << std::endl;
		}

		return true;
	}

	return false;
}

}	// vr


