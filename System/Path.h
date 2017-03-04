#pragma once
/**
@file Path.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.

Implementacja klasy Path. Poniewa¿ Path nie znajduje siê jeszcze w standardzie c++, ale
zostanie do niego w³¹czone dopiero wraz z c++17, to potrzebny jest ten wrapper.
Na razie implementacje dla ró¿nych kompilatorów umieszczaj¹ tê klasê w ró¿nych namespacach,
wiêc nie da siê tego zaimplementowac w pe³ni przenoœnie.
*/

#include <filesystem>
#include <string>
#include <codecvt>

namespace filesystem
{

namespace experimental = std::tr2::sys;

class Path
{
private:
	experimental::path		m_path;

public:

	explicit				Path			();
							Path			( const Path& path );
							Path			( Path&& path );
							Path			( const std::wstring& path );

	template< class Source >
							Path			( const Source& source );


	Path&					operator=		( const Path& other );
	Path&					operator=		( Path&& other );

	Path&					operator/=		( const Path& other );
	bool					operator==		( const Path& other ) const;
	bool					Compare			( const Path& path1, const Path& path2 );

	friend Path				operator/		( const Path& path1, const Path& path2 );

	std::string				String			() const;


	std::string				GetFileName		() const;
	std::string				GetExtension	() const;
	Path					GetParent		() const;
	Path					GetDirectory	() const;

	void					Normalize		();


	bool					HasRoot			() const;
	bool					HasFileName		() const;
	bool					HasExtension	() const;

	bool					IsRelative		() const;
	bool					IsAbsolut		() const;

	/**@brief Checks if file or directory exists on filesystem.*/
	bool					Exists			() const;

	static Path				WorkingDirectory();

public:
	const experimental::path&		GetStdPath		() const;
};


/**@brief */
template< class Source >
inline		Path::Path		( const Source& source )
	:	m_path( experimental::path( source ) )
{}

/**@brief */
inline		Path::Path()
{}

/**@brief */
inline		Path::Path		( const Path& path )
	:	m_path( path.m_path )
{}

/**@brief */
inline		Path::Path		( Path&& path )
	:	m_path( std::move( path.m_path ) )
{}

/**@brief */
inline		Path::Path		( const std::wstring& path )
{
	typedef std::codecvt_utf8< wchar_t > ConvertType;
	std::wstring_convert< ConvertType, wchar_t > converter;
	auto pathStr = converter.to_bytes( path );

	m_path = experimental::path( pathStr );
}

/**@brief */
inline Path&	Path::operator=		( const Path& other )
{
	m_path = other.m_path;
	return *this;
}

/**@brief */
inline Path&	Path::operator=		( Path&& other )
{
	m_path = std::move( other.m_path );
	return *this;
}

/**@brief */
inline Path&	Path::operator/=	( const Path& other )
{
	m_path /= other.m_path;
	return *this;
}

/**@brief */
inline bool		Path::operator==	( const Path& other ) const
{
	return experimental::equivalent( m_path, other.m_path );
}

/**@brief Porównuje œcie¿ki. Przed porównaniem œcie¿ki s¹ normalizowane.*/
inline bool		Path::Compare		( const Path& path1, const Path& path2 )
{
	return experimental::equivalent( path1.m_path, path2.m_path );
}

/**@brief */
inline std::string		Path::String() const
{
	return m_path.string();
}

/**@brief */
inline std::string		Path::GetFileName() const
{
	return m_path.filename().string();
}

/**@brief */
inline std::string		Path::GetExtension() const
{
	return m_path.extension().string();
}

/**@brief */
inline Path				Path::GetParent() const
{
	return m_path.parent_path();
}

/**@brief */
inline Path				Path::GetDirectory() const
{
	if( m_path.has_filename() )
		return GetParent();
	else
		return Path( *this );
}

/**@brief Normalizuje œcie¿kê.
@attention Obecna implementacja mo¿e nie dzia³aæ do koñca.*/
inline void				Path::Normalize()
{
	//m_path = experimental::complete( m_path );
}

/**@brief */
inline bool				Path::HasRoot() const
{
	return m_path.has_root_directory();
}

/**@brief */
inline bool				Path::HasFileName() const
{
	return m_path.has_filename();
}

/**@brief */
inline bool				Path::HasExtension() const
{
	return !GetExtension().empty();
}

/**@brief */
inline bool				Path::IsRelative() const
{
	return !IsAbsolut();
}

/**@brief */
inline bool				Path::IsAbsolut() const
{
	return m_path.has_root_name() && m_path.has_root_directory();
}

/**@brief Check if file exists.*/
inline bool				Path::Exists() const
{
	return experimental::exists( m_path );
}

// ================================ //
//
inline Path				Path::WorkingDirectory()
{
	return experimental::current_path();
}

/**@brief */
inline Path				operator/( const Path& path1, const Path& path2 )
{
	Path newPath = path1;
	newPath /= path2;
	return newPath;
}

/**@brief Returns standard library path.
Use only if you must.*/
inline const experimental::path&	Path::GetStdPath() const
{
	return m_path;
}

}
