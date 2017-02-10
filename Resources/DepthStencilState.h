#pragma once
/**
@file DepthStencilState.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Common/ObjectDeleter.h"
#include "ResourceObject.h"





/**@brief 
@ingroup PipelineState*/
struct DepthStencilInfo
{
	bool			EnableDepthTest;
	bool			EnableStencilTest;

// ================================ //
//
	DepthStencilInfo()
		:	EnableDepthTest( true )
		,	EnableStencilTest( false )
	{}

#define Compare( x ) if( x != other.x ) return false;

	bool	operator==	( const DepthStencilInfo& other ) const
	{
		Compare( EnableDepthTest );
		Compare( EnableStencilTest );
	}

#undef Compare
};




/**@brief 
@ingroup PipelineState*/
class DepthStencilState : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	friend ObjectDeleter< DepthStencilState >;
private:
protected:

	virtual			~DepthStencilState() = default;

public:
	explicit		DepthStencilState()
		:	ResourceObject( 0 )
	{}

	virtual const DepthStencilInfo&		GetDescriptor	() = 0;
};
