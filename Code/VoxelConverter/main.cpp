#include "tclap/CmdLine.h"

#include "swCommonLib/System/Path.h"

#include "VoxelConverter.h"

#include <iostream>
#include <string>



int main( int argc, char** argv )
{
	std::cout << "// ================================================================= //" << std::endl;
	std::cout << "// Program for converting SVO files to VoxelRenderer internal format." << std::endl;
	std::cout << "// ================================================================= //" << std::endl << std::endl;

	try
	{
		TCLAP::CmdLine cmd( "Program for converting SVO files to VoxelRenderer internal format.", ' ', "0.1" );

		TCLAP::ValueArg< std::string > inputFileArg( "i", "input", "Input SVO file", true, "", "file path" );
		TCLAP::ValueArg< std::string > outputFileArg( "o", "output", "Output Voxelrenderer file", true, "", "file path" );

		cmd.add( inputFileArg );
		cmd.add( outputFileArg );

		cmd.parse( argc, argv );

		filesystem::Path inputFile = inputFileArg.getValue();
		filesystem::Path outputFile = outputFileArg.getValue();

		if( !inputFile.Exists() )
		{
			std::cout << "Input Path: [" << inputFile.String() << "] doesn't exist." << std::endl;
			return 1;
		}

		if( inputFile.GetExtension() != ".octree" )
		{
			std::cout << "Input Path: [" << inputFile.String() << "] should have .octree extension." << std::endl;
			return 1;
		}

		std::cout << "Converting file: [" << inputFile.String() << "]" << std::endl;
		std::cout << "Output file: [" << outputFile.String() << "]" << std::endl;

		VoxelConverter converter;
		bool result = converter.Convert( inputFile, outputFile );
		
		if( result )
		{
			std::cout << "Converted successfully." << std::endl;
		}
		else
		{
			std::cout << "Conversion failed." << std::endl;
		}

	}
	catch( TCLAP::ArgException &e )
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}

}

