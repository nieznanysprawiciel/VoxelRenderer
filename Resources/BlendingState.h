#pragma once
/**
@file FileName
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Common/ObjectDeleter.h"

#include "ResourceObject.h"
#include "swGraphicAPI/Rendering/GraphicAPIConstants.h"

#include <DirectXMath.h>



/**@brief Initialization of BlendingState.

@ingroup PipelineState*/
struct BlendingInfo
{
	DirectX::XMFLOAT4	CustomBlendFactor;		///< Custom blend factors.
	bool				EnableBlending;
	BlendOperation		ColorOperation;
	BlendOperation		AlphaOperation;
	BlendFactor			SrcColorBlend;
	BlendFactor			DstColorBlend;
	BlendFactor			SrcAlphaBlend;
	BlendFactor			DstAlphaBlend;

// ================================ //
//
	BlendingInfo()
		:	CustomBlendFactor( DirectX::XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f ) )
		,	EnableBlending( false )
		,	ColorOperation( BlendOperation::Add )
		,	AlphaOperation( BlendOperation::Add )
		,	SrcColorBlend( BlendFactor::One )
		,	DstColorBlend( BlendFactor::One )
		,	SrcAlphaBlend( BlendFactor::One )
		,	DstAlphaBlend( BlendFactor::One )
	{}

#define Compare( x ) if( x != other.x ) return false;

	bool	operator==	( const BlendingInfo& other ) const
	{
		Compare( CustomBlendFactor.x );
		Compare( CustomBlendFactor.y );
		Compare( CustomBlendFactor.z );
		Compare( CustomBlendFactor.w );
		Compare( EnableBlending );
		Compare( ColorOperation );
		Compare( EnableBlending );
		Compare( AlphaOperation );
		Compare( SrcColorBlend );
		Compare( DstColorBlend );
		Compare( SrcAlphaBlend );
		Compare( DstAlphaBlend );
		return true;
	}

#undef Compare
};




/**@brief 

@ingroup PipelineState*/
class BlendingState : public ResourceObject
{
	RTTR_ENABLE( ResourceObject );
	friend ObjectDeleter< BlendingState >;
private:
protected:

	virtual			~BlendingState() = default;

public:
	explicit		BlendingState()
		:	ResourceObject( 0 )
	{}


	virtual const BlendingInfo&		GetDescriptor	() = 0;
};

