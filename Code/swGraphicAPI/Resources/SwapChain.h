#pragma once

#include "swGraphicAPI/Resources/MeshResources.h"


typedef void*	WindowHandler;


struct SwapChainDescriptor
{
	WindowHandler			WindowHandle;
	unsigned short			Width;
	unsigned short			Height;
	ResourceFormat			Format;
	uint8					SamplesCount;
	uint8					SamplesQuality;
	bool					AllowFullscreen;
	uint8					NumBuffers;


	SwapChainDescriptor()
		:	SamplesCount( 1 )
		,	SamplesQuality( 0 )
		,	AllowFullscreen( true )
		,	Format( ResourceFormat::RESOURCE_FORMAT_R8G8B8A8_UNORM )
		,	WindowHandle( nullptr )
		,	NumBuffers( 1 )
	{}
};



class SwapChain : public ResourceObject
{
	RTTR_ENABLE()
protected:

	ResourcePtr< RenderTargetObject >	m_renderTarget;

protected:
	SwapChain( RenderTargetObject* windowRT )
		:	ResourceObject( WRONG_ID )
		,	m_renderTarget( windowRT )
	{
		assert( windowRT );
	}

public:
	virtual ~SwapChain()
	{}


	virtual void			Present			( int syncInterval )					= 0;
	virtual void			Resize			( uint16 newWidth, uint16 newHeight )	= 0;


	ResourcePtr< RenderTargetObject >		GetRenderTarget	()		{ return m_renderTarget; }

	// Inherited via ResourceObject
	virtual std::string		GetResourceName	() const override		{ return "SwapChain: " + m_renderTarget->GetResourceName(); }
};

