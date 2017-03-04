#pragma once
/**
@file IAttribute.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/HierarchicalChunkedFormat/Headers/AttributeHeader.h"


namespace sw
{


/**@brief Base attribute class.

Class is overriden for built in attributes.*/
class IAttribute
{
private:
protected:
public:
	explicit		IAttribute	() = default;
	virtual			~IAttribute	() = default;

	virtual bool			ParseMemory	( DataPtr data, Size size )		= 0;
	virtual bool			Save		( DataPtr data, Size size )		= 0;
	virtual Size			ComputeSize	()								= 0;
	virtual AttributeType	GetType		()								= 0;
};


DEFINE_PTR_TYPE( IAttribute );

}	// sw
