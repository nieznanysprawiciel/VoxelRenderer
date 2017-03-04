#pragma once
/**
@file ChunkNameAttrib.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "IAttribute.h"

#include <string>



namespace sw
{

/**@brief */
class ChunkNameAttrib : public IAttribute
{
public:

	std::string			ChunkName;

protected:
public:
	explicit		ChunkNameAttrib		() = default;
	explicit		ChunkNameAttrib		( const std::string& name );
	explicit		ChunkNameAttrib		( std::string&& name );
	virtual			~ChunkNameAttrib	() = default;


	// Inherited via IAttribute
	virtual bool				ParseMemory		( DataPtr data, Size size ) override;
	virtual bool				Save			( DataPtr data, Size size ) override;
	virtual Size				ComputeSize		()							override;
	virtual AttributeType		GetType			()							override;

};




} // sw


