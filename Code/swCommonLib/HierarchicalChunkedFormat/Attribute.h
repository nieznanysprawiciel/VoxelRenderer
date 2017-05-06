#pragma once
/**
@file Attribute.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/HierarchicalChunkedFormat/Attributes/AttributeTypes.h"

namespace sw
{


class AttributeRepr;
DEFINE_PTR_TYPE( AttributeRepr );


/**@brief 
@ingroup HierarchicalChunkedFormat*/
class Attribute
{
	FRIEND_CLASS_TESTER( HCF );
private:

	AttributeReprPtr		m_attribPtr;

protected:
public:
	explicit		Attribute	() = default;
	explicit		Attribute	( AttributeReprPtr attribPtr );
	~Attribute() = default;


	bool			IsValid		() const;
	AttributeType	GetType		() const;
	Size			GetSize		() const;
};


}	// sw
