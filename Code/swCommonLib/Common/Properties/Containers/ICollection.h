#pragma once

#include "swCommonLib/Common/TypesDefinitions.h"
#include "swCommonLib/Common/Properties/IEnableProperty.h"
#include "swCommonLib/Common/RTTR.h"


template< typename ElementType = IEnableProperty* >
class ICollection	: public IEnableProperty
{
private:
public:

	virtual ~ICollection() = 0 {};

	virtual	void			Add		( ElementType object )				= 0;
	virtual void			Insert	( ElementType object, int32 index )	= 0;
	virtual void			Remove	( ElementType object )				= 0;
	virtual void			Remove	( int32 index )						= 0;
	virtual void			Move	( int32 srcIdx, int32 dstIdx )		= 0;

	virtual int32			Find	( ElementType object )				= 0;

	virtual void			Clear	() = 0;
	virtual int32			Length	() = 0;

	virtual ElementType&	operator[]( int32 idx )						= 0;
};
RTTR_DECLARE_STANDARD_META_TYPE_VARIANTS( ICollection< IEnableProperty* > )
