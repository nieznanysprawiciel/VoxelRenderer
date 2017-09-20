#pragma once
/**
@file Dir.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "Path.h"



namespace filesystem
{


/**@brief */
class Dir
{
private:
protected:
public:
	explicit		Dir() = default;
	~Dir() = default;


	static bool			CreateDirectory		( const filesystem::Path& path );
};


}
