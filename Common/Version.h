#pragma once
/**
@file Version.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"

#include <string>


/**@brief Standard version structure.

Based on: http://sourcey.com/comparing-version-strings-in-cpp/
*/
struct Version
{
	uint32			Major;
	uint32			Minor;
	uint32			Revision;
	uint32			Build;

// ================================ //
//
	/**@brief Contructs Lowest version.*/
	Version()
	{
		Major = 1;
		Minor = 0;
		Revision = 0;
		Build = 0;
	}

	/**@brief */
	Version				( const std::string& version )
	{
		std::sscanf( version.c_str(), "%d.%d.%d.%d", &Major, &Minor, &Revision, &Build );
		if( Major < 0 ) Major = 0;
		if( Minor < 0 ) Minor = 0;
		if( Revision < 0 ) Revision = 0;
		if( Build < 0 ) Build = 0;
	}

	/**@brief Version comparision.*/
	bool operator<		( const Version& other )
	{
		if( Major < other.Major )
			return true;
		else if( Major == other.Major )
		{
			if( Minor < other.Minor )
				return true;
			else if( Minor == other.Minor )
			{
				if( Revision < other.Revision )
					return true;
				else if( Build == other.Build && Build < other.Build )
					return true;
			}
		}

		return false;
	}

	/**@brief Version comparision.*/
	bool operator==		( const Version& other )
	{
		return Major == other.Major
			&& Minor == other.Minor
			&& Revision == other.Revision
			&& Build == other.Build;
	}

	/**@brief */
	friend std::ostream& operator<<		( std::ostream& stream, const Version& ver )
	{
		stream << ver.Major;
		stream << '.';
		stream << ver.Minor;
		stream << '.';
		stream << ver.Revision;
		stream << '.';
		stream << ver.Build;
		return stream;
	}
};

