#pragma once
/**
@file PipelineState.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "DX11Initializer/DX11APIObjects.h"

#include "swGraphicAPI/Resources/BlendingState.h"
#include "swGraphicAPI/Resources/DepthStencilState.h"
#include "swGraphicAPI/Resources/RasterizerState.h"


// ComPtr
#include <wrl/client.h>
using namespace Microsoft::WRL;



/**@brief RasterizerState DirectX11.
@ingroup DX11API*/
class DX11RasterizerState : public RasterizerState, protected DX11APIObjects
{
	RTTR_ENABLE( RasterizerState );
private:
	ComPtr< ID3D11RasterizerState >		m_state;
	RasterizerStateInfo					m_info;

protected:
	~DX11RasterizerState() = default;
public:
	explicit	DX11RasterizerState	( ComPtr< ID3D11RasterizerState > state, const RasterizerStateInfo& info );
	
	ID3D11RasterizerState*				Get	()	{ return m_state.Get(); }

	// Inherited via RasterizerState
	virtual std::string					GetResourceName	() const override;
	virtual const RasterizerStateInfo&	GetDescriptor	() override;

	static DX11RasterizerState*			Create			( const RasterizerStateInfo& info );
};


/**@brief Depth stencil state DirectX11.
@ingroup DX11API*/
class DX11DepthStencilState : public DepthStencilState, public DX11APIObjects
{
	RTTR_ENABLE( DepthStencilState );
private:
	ComPtr< ID3D11DepthStencilState >	m_state;
	DepthStencilInfo					m_info;

protected:
	~DX11DepthStencilState() = default;
public:
	explicit	DX11DepthStencilState	( ComPtr< ID3D11DepthStencilState > state, const DepthStencilInfo& info );
	
	ID3D11DepthStencilState*			Get	()	{ return m_state.Get(); }

	// Inherited via DepthStencilState
	virtual std::string					GetResourceName	() const override;
	virtual const DepthStencilInfo&		GetDescriptor	() override;

	static DX11DepthStencilState*		Create			( const DepthStencilInfo& info );
};


/**@brief Blending state DirectX11.
@ingroup DX11API*/
class DX11BlendingState : public BlendingState, public DX11APIObjects
{
	RTTR_ENABLE( BlendingState );
private:
	ComPtr< ID3D11BlendState >	m_state;
	BlendingInfo				m_info;

protected:
	~DX11BlendingState() = default;
public:
	explicit	DX11BlendingState	( ComPtr< ID3D11BlendState > state, const BlendingInfo& info );
	
	ID3D11BlendState*				Get	()	{ return m_state.Get(); }

	// Inherited via BlendingState
	virtual std::string				GetResourceName	() const override;
	virtual const BlendingInfo&		GetDescriptor	() override;

	static DX11BlendingState*		Create			( const BlendingInfo& info );
};

