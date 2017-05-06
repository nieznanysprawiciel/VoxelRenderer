#pragma once
/**
@file ConfigHelper.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


/**@brief Helpers for writing configuration files.

These macros should be placed in class definition. They create field named "m_name"
and expose getter function name. You can specify default value.

@todo Write base class for configs and make automatic serialization and deserialization.*/



#define DEFINE_CONFIG_VALUE_DEFAULT( type, name, defaultValue )		\
private:															\
	type				m_##name = defaultValue;					\
public:																\
	type			name		() const { return m_##name ; }


#define DEFINE_CONFIG_VALUE( type, name )		\
private:										\
	type				m_##name;				\
public:											\
	type			name		() const { return m_##name ; }


