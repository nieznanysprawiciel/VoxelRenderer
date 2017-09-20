/**
@file Dir.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "Dir.h"

#include <filesystem>


namespace filesystem
{


// ================================ //
//
bool			Dir::CreateDirectory		( const filesystem::Path& path )
{
	if( path.HasFileName() )
		return std::experimental::filesystem::create_directories( path.GetParent().String() );
	else
		return std::experimental::filesystem::create_directories( path.String() );
}

}
