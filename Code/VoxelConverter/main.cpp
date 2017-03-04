#include "tclap/CmdLine.h"

#include "swCommonLib/System/Path.h"


#include <iostream>
#include <string>



int main( int argc, char** argv )
{
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

		
		
	}
	catch( TCLAP::ArgException &e )
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}

}

