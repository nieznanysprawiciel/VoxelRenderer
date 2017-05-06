#include "swCommonLib/Tests/TestHCF/stdafx.h"
#include "swCommonLib/HierarchicalChunkedFormat/HCF.h"

#include "TesterClasses/TesterHCF.h"



#include "swCommonLib/External/Catch/catch.hpp"


#include <numeric>


using namespace sw;


const Size DataElementsCount = 2000;
typedef int DataElementType;


#include "TesterClasses/TestAttribs.h"



// ================================ //
//
TEST_CASE( "HCF - Stream position test" )
{
	std::vector< DataElementType > writeData( DataElementsCount );
	std::iota( writeData.begin(), writeData.end(), 1 );

	sw::HCF hcf;
	bool openResult = hcf.OpenFile( "HCF/StreamPosTest.hcf", HCF::WriteMode::DirectToFile );
	REQUIRE( openResult );


	VoxtreeHeader header;
	header.VoxtreeVersion.Major = 1;
	header.VoxtreeVersion.Minor = 0;
	header.GridSize = 1024;
	header.FreeIndirectOffset = 3;

	Attribute attrib = hcf.AddAttribute( header );
	CHECK( CLASS_TESTER( HCF )::ValidateStreamPos( &hcf, attrib ) );

	sw::Chunk root = hcf.CreateRootChunk();
	root.Fill( (sw::DataPtr)writeData.data(), writeData.size() * sizeof( DataElementType ) );

	CHECK( CLASS_TESTER( HCF )::ValidateStreamPos( &hcf, root ) );

	bool result = hcf.WriteFile( "HCF/StreamPosTest.hcf" );
	CHECK( result );
}


// ================================ //
//
TEST_CASE( "HCF - Stream position test 2" )
{
	std::vector< DataElementType > writeData( DataElementsCount );
	std::iota( writeData.begin(), writeData.end(), 1 );

	sw::HCF hcf;
	bool openResult = hcf.OpenFile( "HCF/StreamPosTest.hcf", HCF::WriteMode::DirectToFile );
	REQUIRE( openResult );


	VoxtreeHeader header;
	header.VoxtreeVersion.Major = 1;
	header.VoxtreeVersion.Minor = 0;
	header.GridSize = 1024;
	header.FreeIndirectOffset = 3;

	Attribute attrib = hcf.AddAttribute( header );
	CHECK( CLASS_TESTER( HCF )::ValidateStreamPos( &hcf, attrib ) );

	attrib = hcf.AddAttribute( sw::HCF::UserDefinedAttributes + 2, header );
	CHECK( CLASS_TESTER( HCF )::ValidateStreamPos( &hcf, attrib ) );

	attrib = hcf.AddAttribute( sw::HCF::UserDefinedAttributes + 3, header );
	CHECK( CLASS_TESTER( HCF )::ValidateStreamPos( &hcf, attrib ) );


	sw::Chunk root = hcf.CreateRootChunk();
	
	Chunk chunk1 = root.CreateChunk();
	chunk1.Fill( (sw::DataPtr)writeData.data(), writeData.size() * sizeof( DataElementType ) );

	CHECK( CLASS_TESTER( HCF )::ValidateStreamPos( &hcf, chunk1 ) );

	Chunk chunk2 = root.CreateChunk();
	attrib = chunk2.AddAttribute( sw::HCF::UserDefinedAttributes + 3, header );
	CHECK( CLASS_TESTER( HCF )::ValidateStreamPos( &hcf, attrib ) );

	chunk2.Fill( (sw::DataPtr)writeData.data(), writeData.size() * sizeof( DataElementType ) );

	CHECK( CLASS_TESTER( HCF )::ValidateStreamPos( &hcf, chunk2 ) );

	bool result = hcf.WriteFile( "HCF/StreamPosTest.hcf" );
	CHECK( result );
}



