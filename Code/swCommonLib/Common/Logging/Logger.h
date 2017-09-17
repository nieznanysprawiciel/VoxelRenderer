#pragma once
/**
@file Logger.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include <string>


namespace sw
{

namespace detail
{
	class LoggerImpl;
}


/**@brief This class is interface to write logger independent system.

Use your favourite logger internally.*/
class Logger
{
private:

	detail::LoggerImpl*			m_impl;

protected:
public:
	explicit		Logger		() = default;
	~Logger	() = default;


	void			Trace		( const std::string& message );
	void			Debug		( const std::string& message );
	void			Info		( const std::string& message );
	void			Warning		( const std::string& message );
	void			Error		( const std::string& message );
	void			Critical	( const std::string& message );

	void			Trace		( const char* message );
	void			Debug		( const char* message );
	void			Info		( const char* message );
	void			Warning		( const char* message );
	void			Error		( const char* message );
	void			Critical	( const char* message );
};


}	// sw

