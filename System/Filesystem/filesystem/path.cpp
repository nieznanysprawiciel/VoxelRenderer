#include "path.h"


#if defined(_WIN32)
# include <windows.h>
#else
# include <unistd.h>
#endif
#include <sys/stat.h>

#if defined(__linux)
# include <linux/limits.h>
#endif






NAMESPACE_BEGIN(filesystem)



// ================================ //
//
path_impl		path_impl::make_absolute() const
{
#if !defined(_WIN32)
	char temp[ PATH_MAX ];
	if( realpath( str().c_str(), temp ) == NULL )
		throw std::runtime_error( "Internal error in realpath(): " + std::string( strerror( errno ) ) );
	return path( temp );
#else
	std::wstring value = wstr(), out( MAX_PATH, '\0' );
	DWORD length = GetFullPathNameW( value.c_str(), MAX_PATH, &out[ 0 ], NULL );
	if( length == 0 )
		throw std::runtime_error( "Internal error in realpath(): " + std::to_string( GetLastError() ) );
	return path_impl( out.substr( 0, length ) );
#endif
}


// ================================ //
//
bool		path_impl::exists() const
{
#if defined(_WIN32)
	return GetFileAttributesW( wstr().c_str() ) != INVALID_FILE_ATTRIBUTES;
#else
	struct stat sb;
	return stat( str().c_str(), &sb ) == 0;
#endif
}


// ================================ //
//
size_t			path_impl::file_size() const
{
#if defined(_WIN32)
	struct _stati64 sb;
	if( _wstati64( wstr().c_str(), &sb ) != 0 )
		throw std::runtime_error( "path::file_size(): cannot stat file \"" + str() + "\"!" );
#else
	struct stat sb;
	if( stat( str().c_str(), &sb ) != 0 )
		throw std::runtime_error( "path::file_size(): cannot stat file \"" + str() + "\"!" );
#endif
	return (size_t)sb.st_size;
}


// ================================ //
//
bool			path_impl::is_directory() const
{
#if defined(_WIN32)
	DWORD result = GetFileAttributesW( wstr().c_str() );
	if( result == INVALID_FILE_ATTRIBUTES )
		return false;
	return ( result & FILE_ATTRIBUTE_DIRECTORY ) != 0;
#else
	struct stat sb;
	if( stat( str().c_str(), &sb ) )
		return false;
	return S_ISDIR( sb.st_mode );
#endif
}


// ================================ //
//
bool			path_impl::is_file() const
{
#if defined(_WIN32)
	DWORD attr = GetFileAttributesW( wstr().c_str() );
	return ( attr != INVALID_FILE_ATTRIBUTES && ( attr & FILE_ATTRIBUTE_DIRECTORY ) == 0 );
#else
	struct stat sb;
	if( stat( str().c_str(), &sb ) )
		return false;
	return S_ISREG( sb.st_mode );
#endif
}


// ================================ //
//
bool		path_impl::remove_file()
{
#if !defined(_WIN32)
	return std::remove( str().c_str() ) == 0;
#else
	return DeleteFileW( wstr().c_str() ) != 0;
#endif
}


// ================================ //
//
bool			path_impl::resize_file( size_t target_length )
{
#if !defined(_WIN32)
	return ::truncate( str().c_str(), (off_t)target_length ) == 0;
#else
	HANDLE handle = CreateFileW( wstr().c_str(), GENERIC_WRITE, 0, nullptr, 0, FILE_ATTRIBUTE_NORMAL, nullptr );
	if( handle == INVALID_HANDLE_VALUE )
		return false;
	LARGE_INTEGER size;
	size.QuadPart = (LONGLONG)target_length;
	if( SetFilePointerEx( handle, size, NULL, FILE_BEGIN ) == 0 )
	{
		CloseHandle( handle );
		return false;
	}
	if( SetEndOfFile( handle ) == 0 )
	{
		CloseHandle( handle );
		return false;
	}
	CloseHandle( handle );
	return true;
#endif
}


// ================================ //
//
path_impl			path_impl::getcwd()
{
#if !defined(_WIN32)
	char temp[ PATH_MAX ];
	if( ::getcwd( temp, PATH_MAX ) == NULL )
		throw std::runtime_error( "Internal error in getcwd(): " + std::string( strerror( errno ) ) );
	return path( temp );
#else
	std::wstring temp( MAX_PATH, '\0' );
	if( !_wgetcwd( &temp[ 0 ], MAX_PATH ) )
		throw std::runtime_error( "Internal error in getcwd(): " + std::to_string( GetLastError() ) );
	return path_impl( temp.c_str() );
#endif
}

// ================================ //
//
std::wstring path_impl::wstr( path_type type ) const
{
	std::string temp = str( type );
	int size = MultiByteToWideChar( CP_UTF8, 0, &temp[ 0 ], (int)temp.size(), NULL, 0 );
	std::wstring result( size, 0 );
	MultiByteToWideChar( CP_UTF8, 0, &temp[ 0 ], (int)temp.size(), &result[ 0 ], size );
	return result;
}


// ================================ //
//
void path_impl::set( const std::wstring & wstring, path_type type )
{
	std::string string;
	if( !wstring.empty() )
	{
		int size = WideCharToMultiByte( CP_UTF8, 0, &wstring[ 0 ], (int)wstring.size(),
										NULL, 0, NULL, NULL );
		string.resize( size, 0 );
		WideCharToMultiByte( CP_UTF8, 0, &wstring[ 0 ], (int)wstring.size(),
							 &string[ 0 ], size, NULL, NULL );
	}
	set( string, type );
}



// ================================ //
//
bool create_directory( const path_impl & p )
{
#if defined(_WIN32)
	return CreateDirectoryW( p.wstr().c_str(), NULL ) != 0;
#else
	return mkdir( p.str().c_str(), S_IRUSR | S_IWUSR | S_IXUSR ) == 0;
#endif
}


NAMESPACE_END(filesystem)

