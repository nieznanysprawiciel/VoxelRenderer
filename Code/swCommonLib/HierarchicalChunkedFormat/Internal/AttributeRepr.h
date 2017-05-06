#pragma once
/**
@file AttributeRepr.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/HierarchicalChunkedFormat/Headers/AttributeHeader.h"

namespace sw
{

class AttributeRepr;
DEFINE_PTR_TYPE( AttributeRepr );
DEFINE_WPTR_TYPE( AttributeRepr );

class ImplHCF;



/**@brief */
class AttributeRepr
{
	FRIEND_CLASS_TESTER( HCF );
	FRIEND_CLASS_TESTER( AttributeRepr );
private:
	ImplHCF*			m_hcf;

	AttributeHeader		m_header;		///< Header from file.
	AttributeReprPtr	m_nextAttrib;	///< Next attribute in list.

	Size				m_absolutOffset;	///< Offset from beginning of file.

public:
	explicit		AttributeRepr				( ImplHCF* hcf, AttributeType type );
					~AttributeRepr				() = default;


	AttributeHeader&		AccessHeader		() { return m_header; }

	void					FillAttribute		( const DataPtr data, Size dataSize );
	void					AddNextAttribute	( AttributeReprPtr& attribPtr );
};

}	// sw
