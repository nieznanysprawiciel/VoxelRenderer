/**
@file File.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "File.h"


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
	return experimental::file_size( m_filePath.GetStdPath() );
}

/**@brief */
bool		filesystem::File::Remove	()
{
	return experimental::remove( m_filePath.GetStdPath() );
}

/**@brief */
bool		filesystem::File::Move		( const Path& newPath )
{
	std::error_code error;
	experimental::rename( m_filePath.GetStdPath(), newPath.GetStdPath(), error );
	
	if( error )
		return false;
	return true;
}

/**@brief */
bool		filesystem::File::Copy		( const Path& newPath )
{
	return experimental::copy_file( m_filePath.GetStdPath(), newPath.GetStdPath() );
}
