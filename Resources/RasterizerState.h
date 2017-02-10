#pragma once
/**
@file RasterizerStateObject.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Common/ObjectDeleter.h"
#include "ResourceObject.h"


/**@defgroup PipelineState
@ingroup Resources
*/



/**@brief 
@ingroup PipelineState*/
struct RasterizerStateInfo
{
	CullMode		CullMode;
	FillMode		FillMode;
	int				DepthBias;
	bool			IsClockwise;
	bool			EnableScissor;
	bool			EnableZClipping;			///< Clipping of far plane.
	bool			ConservativeRasterizer;		///< Since in DirectX 11.2


// ================================ //
//
	RasterizerStateInfo()
		:	CullMode( CullMode::Back )
		,	FillMode( FillMode::Solid )
		,	DepthBias( 0 )
		,	IsClockwise( false )
		,	EnableScissor( false )
		,	EnableZClipping( true )
		,	ConservativeRasterizer( false )
	{}

#define Compare( x ) if( x != other.x ) return false;


	bool	operator==	( const RasterizerStateInfo& other ) const
	{
		Compare( CullMode );
		Compare( FillMode );
		Compare( DepthBias );
		Compare( IsClockwise );
		Compare( EnableScissor );
		Compare( EnableZClipping );
		Compare( ConservativeRasterizer );
		return true;
	}

#undef Compare
};




/**@brief Rasterizer state and depth stencil state.

@ingroup PipelineState*/
class RasterizerState : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	friend ObjectDeleter< RasterizerState >;
private:
protected:
	virtual			~RasterizerState() = default;
public:
	explicit		RasterizerState()
		:	ResourceObject( 0 )
	{}


	virtual const RasterizerStateInfo&		GetDescriptor	() = 0;
};

