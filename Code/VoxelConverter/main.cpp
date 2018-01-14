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
		TCLAP::ValueArg< std::string > outputFileArg( "o", "output", "Output VoxelRenderer file", true, "", "file path" );
		TCLAP::ValueArg< std::string > textureFileArg( "t", "texture", "Input texture to use on octree", false, "", "file path" );
		TCLAP::ValueArg< std::string > textureFilteringArg( "f", "filter", "Filter used to texture octree", false, "Bilinear", "Filter type string" );
		
		TCLAP::SwitchArg flipUArg( "u", "flipU", "Flip U coordinate of texture", false );
		TCLAP::SwitchArg flipVArg( "v", "flipV", "Flip V coordinate of texture", false );

		cmd.add( inputFileArg );
		cmd.add( outputFileArg );
		cmd.add( textureFileArg );
		cmd.add( textureFilteringArg );

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

		if( textureFileArg.isSet() )
		{
			filesystem::Path textureFile = textureFileArg.getValue();

			if( !textureFile.Exists() )
			{
				std::cout << "Texture Path: [" << textureFile.String() << "] doesn't exist." << std::endl;
				return 1;
			}

			std::cout << "Texture file: [" << textureFile.String() << "]" << std::endl;

			converter.AddTexture( textureFile );
			converter.FlipUV( flipUArg.getValue(), flipVArg.getValue() );

			std::string filterName = textureFilteringArg.getValue();
			
			if( filterName == "Bilinear" )
				converter.SetSampler( SamplerType::Bilinear );
			else if( filterName == "Point" )
				converter.SetSampler( SamplerType::Point );
			else
				std::cout << "Unknown filter: [" << filterName << "]. Using default instead." << std::endl;
		}

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

