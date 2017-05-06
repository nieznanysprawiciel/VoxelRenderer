#include "swCommonLib/Tests/TestHCF/stdafx.h"
#include "swCommonLib/HierarchicalChunkedFormat/HCF.h"

#include "swCommonLib/External/Catch/catch.hpp"

#include "TesterClasses/TestAttribs.h"
#include "TesterClasses/TesterHCF.h"

#include <numeric>


struct ExampleRealFormatHeader
{
	char			HeaderLetters[ 3 ];
	Version			Version;

// ================================ //
//
	ExampleRealFormatHeader()
	{
		HeaderLetters[ 0 ] = 'B';
		HeaderLetters[ 1 ] = 'L';
		HeaderLetters[ 2 ] = 'A';

		Version.Major = 3;
		Version.Minor = 123;
		Version.Revision = 1111;
		Version.Build = 120542;
	}
};

template<>
sw::AttributeType	GetAttributeTypeID< ExampleRealFormatHeader >() { return 0x10000;  }


using namespace sw;


const Size DataElementsCount = 2000;
typedef int DataElementType;




// ================================ //
//
TEST_CASE( "HCF - Simple write test" )
{
	std::vector< DataElementType > writeData( DataElementsCount );
	std::iota( writeData.begin(), writeData.end(), 1 );

	sw::HCF hcf;
	bool openResult = hcf.OpenFile( "HCF/SimpleWriteTest.hcf", HCF::WriteMode::DirectToFile );
	REQUIRE( openResult );

	Attribute topAttribute = hcf.AddAttribute( ExampleRealFormatHeader() );
	CHECK( topAttribute.IsValid() );
	CHECK( topAttribute.GetType() == GetAttributeTypeID< ExampleRealFormatHeader >() );

	Chunk rootChunk = hcf.CreateRootChunk();
	REQUIRE( rootChunk.IsValid() );


	// Can't add attribute after root chunk creation.
	Attribute topAttribute2 = hcf.AddAttribute( ExampleRealFormatHeader() );
	CHECK( !topAttribute2.IsValid() );


	std::string rootChunkName = "rootChunk";
	Attribute attrib = rootChunk.AddAttribute( AttributeTypeBuiltIn::ChunkName, (const DataPtr)rootChunkName.c_str(), rootChunkName.length() + 1 );

	REQUIRE( attrib.IsValid() );

	Chunk chunk1 = rootChunk.CreateChunk();
	REQUIRE( chunk1.IsValid() );

	// Can't add attribute after child chunk creation.
	Attribute attrib2 = rootChunk.AddAttribute( ExampleRealFormatHeader() );
	CHECK( !attrib2.IsValid() );


	Chunk nested1 = chunk1.CreateChunk();
	REQUIRE( nested1.IsValid() );

	// Can't fille chunk after child chunk creation.
	bool fillResult = chunk1.Fill( (const DataPtr)writeData.data(), sizeof( int ) * writeData.size() );
	CHECK( !fillResult );


	nested1.Fill( (const DataPtr)writeData.data(), sizeof( int ) * writeData.size() );


	Chunk nested2 = chunk1.CreateChunk();
	REQUIRE( nested2.IsValid() );
	
	nested2.Fill( (const DataPtr)writeData.data(), sizeof( int ) * writeData.size() );



	Chunk chunk2 = rootChunk.CreateChunk();
	REQUIRE( chunk2.IsValid() );

	chunk2.Fill( (const DataPtr)writeData.data(), sizeof( int ) * writeData.size() );

	bool result = hcf.WriteFile( "HCF/WriteTest.hcf" );

	CHECK( result );
}


// ================================ //
//
TEST_CASE( "HCF - Simple load test" )
{
	// Comparision data.
	std::vector< DataElementType > readData( DataElementsCount );
	std::iota( readData.begin(), readData.end(), 1 );

	sw::HCF hcf;
	bool openResult = hcf.LoadFile( "HCF/SimpleReadTest.hcf", sw::HCF::READ_ONLY );
	REQUIRE( openResult );

	Chunk root = hcf.GetRootChunk();
	REQUIRE( root.IsValid() );

	Chunk afterRoot = root.NextChunk();
	CHECK_FALSE( afterRoot.IsValid() );
	
	CHECK( root.HasChildren() );

	Chunk chunk1 = root.FirstChild();
	REQUIRE( chunk1.IsValid() );
	CHECK( chunk1.HasChildren() );

	// Check nested chunks.
	int numChunks = 0;

	Chunk childIter = chunk1.FirstChild();
	while( childIter.IsValid() )
	{
		numChunks++;

		CHECK_FALSE( childIter.HasChildren() );
		CHECK_FALSE( childIter.FirstChild().IsValid() );
		CHECK( chunk1.ParentChunk() == root );

		DataPack data = childIter.AccessData();
		
		// Check content.
		CHECK( data.DataSize == DataElementsCount * sizeof( DataElementType ) );
		REQUIRE( data.Data );

		//  This should contain the same pointer.
		DataPack dataSecondCopy = childIter.AccessData();
		CHECK( dataSecondCopy.DataSize == DataElementsCount * sizeof( DataElementType ) );
		CHECK( dataSecondCopy.Data == data.Data );


		// Compare loaded data with reference
		bool equal = true;
		DataElementType* typedData = (DataElementType*)data.Data;
		
		for( size_t i = 0; i < DataElementsCount; i++ )
		{
			if( readData[ i ] != typedData[ i ] )
				equal = false;
		}
		CHECK( equal );

		// Steal data from chunk
		DataUPack dataOwner = childIter.StealData();
		CHECK( dataOwner.DataSize == DataElementsCount * sizeof( DataElementType ) );
		CHECK( dataOwner.Data );

		// Call access data for the second time to prove, that previously data has been stolen properly.
		DataPack dataThird = childIter.AccessData();
		CHECK( dataThird.DataSize == DataElementsCount * sizeof( DataElementType ) );
		CHECK( dataThird.Data != data.Data );
		CHECK( dataThird.Data != dataSecondCopy.Data );


		childIter = childIter.NextChunk();
	}

	CHECK( numChunks == 2 );
}

// ================================ //
//
TEST_CASE( "HCF - Root chunk write test" )
{
	std::vector< DataElementType > writeData( DataElementsCount );
	std::iota( writeData.begin(), writeData.end(), 1 );

	sw::HCF hcf;
	bool openResult = hcf.OpenFile( "HCF/WriteToRoot.hcf", HCF::WriteMode::DirectToFile );
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

	bool result = hcf.WriteFile( "HCF/WriteToRoot.hcf" );
	CHECK( result );

//====================================================================================//
//			Load	
//====================================================================================//

	sw::HCF loadHCF;
	openResult = loadHCF.LoadFile( "HCF/WriteToRoot.hcf", sw::HCF::READ_ONLY );
	REQUIRE( openResult );

	root = loadHCF.GetRootChunk();
	REQUIRE( root.IsValid() );

	DataPack data = root.AccessData();
		
	// Check content.
	REQUIRE( data.Data );
	CHECK( data.DataSize == DataElementsCount * sizeof( DataElementType ) );

	// Compare loaded data with reference
	bool equal = true;
	DataElementType* typedData = (DataElementType*)data.Data;
		
	for( size_t i = 0; i < DataElementsCount; i++ )
	{
		if( writeData[ i ] != typedData[ i ] )
			equal = false;
	}
	CHECK( equal );
}

