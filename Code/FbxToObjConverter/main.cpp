#include "tclap/CmdLine.h"

#include "swCommonLib/System/Path.h"

#include "Converter.h"

#include <iostream>
#include <string>



int main( int argc, char** argv )
{
	std::cout << "// ================================================================= //" << std::endl;
	std::cout << "// Program for converting FBX files to OBJ." << std::endl;
	std::cout << "// ================================================================= //" << std::endl << std::endl;

	try
	{
		TCLAP::CmdLine cmd( "Program for converting FBX files to OBJ.", ' ', "0.1" );

		TCLAP::ValueArg< std::string > inputFileArg( "i", "input", "Input FBX file", true, "", "file path" );
		TCLAP::ValueArg< std::string > outputFileArg( "o", "output", "Output OBJ file", true, "", "file path" );
		TCLAP::SwitchArg generateMatfile( "m", "generateMatfile", "Generate file with material paths.", false );

		cmd.add( inputFileArg );
		cmd.add( outputFileArg );
		cmd.add( generateMatfile );

		cmd.parse( argc, argv );

		filesystem::Path inputFile = inputFileArg.getValue();
		filesystem::Path outputFile = outputFileArg.getValue();
		
		if( !inputFile.Exists() )
		{
			std::cout << "Input Path: [" << inputFile.String() << "] doesn't exist." << std::endl;
			return 1;
		}

		std::cout << "Converting file: [" << inputFile.String() << "]" << std::endl;
		std::cout << "Output file: [" << outputFile.String() << "]" << std::endl;

		bool result = vr::Converter::Convert( inputFile, outputFile, generateMatfile.getValue() );

		if( result )
		{
			std::cout << "Conversion succeded." << std::endl;
			return 0;
		}
		else
		{
			std::cout << "Conversion failed." << std::endl;
			return 1;
		}

	}
	catch( TCLAP::ArgException &e )
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}

}

