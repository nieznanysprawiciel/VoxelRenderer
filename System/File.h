#pragma once
/**
@file File.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"
#include "Path.h"


namespace filesystem
{


/**@brief File abstraction layer.

Encapsulates experimental c++ features. Provides interface for operations on files.
Maybe in future we can delete this class and use standard library.*/
class File
{
private:

	Path			m_filePath;

protected:
public:
	explicit		File	( const Path& path );
	explicit		File	( Path&& path );
					~File	() = default;

	bool			Exists		() const;
	Size			FileSize	() const;

	bool			Remove		();
	bool			Move		( const Path& newPath );
	bool			Copy		( const Path& newPath );
};

}	// filesystem

