/**
@file File.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "File.h"

#include <fstream>
#include <sstream>
#include <string>

#include <filesystem>

namespace experimental = std::tr2::sys;


// ================================ //
//
experimental::path		GetStdPath( const filesystem::Path& path )
{
	return experimental::path( path.WString() );
}


// ================================ //
//
filesystem::File::File		( const Path& path )
	:	m_filePath( path )
{ }

// ================================ //
//
filesystem::File::File		( Path&& path )
	:	m_filePath( std::move( path ) )
{}

/**@brief */
bool		filesystem::File::Exists	() const
{
	return m_filePath.Exists();
}

/**@brief */
Size		filesystem::File::FileSize	() const
{
	return experimental::file_size( GetStdPath( m_filePath ) );
}

/**@brief */
bool		filesystem::File::Remove	()
{
	return experimental::remove( GetStdPath( m_filePath ) );
}

/**@brief */
bool		filesystem::File::Move		( const Path& newPath )
{
	std::error_code error;
	experimental::rename( GetStdPath( m_filePath ), GetStdPath( newPath ), error );
	
	if( error )
		return false;
	return true;
}

/**@brief */
bool		filesystem::File::Copy		( const Path& newPath )
{
	return experimental::copy_file( GetStdPath( m_filePath ), GetStdPath( newPath ) );
}

// ================================ //
//
std::string			filesystem::File::Load		( const Path& newPath )
{
	std::ifstream file( newPath.String() );
	std::stringstream buffer;
	buffer << file.rdbuf();

	return buffer.str();
}
