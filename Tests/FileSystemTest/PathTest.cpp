#include "swCommonLib/External/Catch/catch.hpp"

#include "swCommonLib/System/Path.h"







TEST_CASE( "Path - Comparision operator" )
{
	filesystem::Path reference( "assets/wombat.jpg" );
	filesystem::Path difference( "assets/wombat2.jpg" );

	filesystem::Path newPath( reference );

	CHECK( reference == newPath );
	CHECK( reference.String() == newPath.String() );
	CHECK( reference.WString() == newPath.WString() );

	CHECK( difference != newPath );
	CHECK( difference.String() != newPath.String() );
	CHECK( difference.WString() != newPath.WString() );
}


// ================================ //
// Path should return right result for examples from http://en.cppreference.com/w/cpp/experimental/fs/path/extension
TEST_CASE( "Path - Aquire Extension" )
{
	filesystem::Path path1( "/foo/bar.txt" );
	filesystem::Path path2( "/foo/bar." );
	filesystem::Path path3( "/foo/bar" );
	filesystem::Path path4( "/foo/bar.txt/bar.cc" );
	filesystem::Path path5( "/foo/bar.txt/bar." );
	filesystem::Path path6( "/foo/bar.txt/bar" );
	filesystem::Path path7( "/foo/." );
	filesystem::Path path8( "/foo/.." );
	filesystem::Path path9( "/foo/.hidden" );

	CHECK( path1.GetExtension() == ".txt" );
	CHECK( path2.GetExtension() == "." );
	CHECK( path3.GetExtension() == "" );
	CHECK( path4.GetExtension() == ".cc" );
	CHECK( path5.GetExtension() == "." );
	CHECK( path6.GetExtension() == "" );
	CHECK( path7.GetExtension() == "" );
	CHECK( path8.GetExtension() == "" );
	CHECK( path9.GetExtension() == ".hidden" );
}

