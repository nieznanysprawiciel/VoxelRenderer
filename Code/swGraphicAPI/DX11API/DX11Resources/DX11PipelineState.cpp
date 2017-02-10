/**
@file DX11PipelineState.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/
#include "swGraphicAPI/DX11API/stdafx.h"

#include "DX11PipelineState.h"
#include "DX11Initializer/DX11ConstantsMapper.h"


RTTR_REGISTRATION
{
	rttr::registration::class_< DX11RasterizerState >( "DX11RasterizerState" );
	rttr::registration::class_< DX11DepthStencilState >( "DX11DepthStencilState" );
	rttr::registration::class_< DX11BlendingState >( "DX11BlendingState" );
}



//====================================================================================//
//			RasterizerState	
//====================================================================================//

DX11RasterizerState::DX11RasterizerState( ComPtr< ID3D11RasterizerState > state, const RasterizerStateInfo& info )
	:	m_state( state )
	,	m_info( info )
{ }

// ================================ //
//
std::string						DX11RasterizerState::GetResourceName() const
{
	return std::string();
}

// ================================ //
//
const RasterizerStateInfo&		DX11RasterizerState::GetDescriptor()
{
	return m_info;
}

// ================================ //
//
DX11RasterizerState*			DX11RasterizerState::Create		( const RasterizerStateInfo& info )
{
	D3D11_RASTERIZER_DESC desc;
	desc.CullMode = DX11ConstantsMapper::Get( info.CullMode );
	desc.FillMode = DX11ConstantsMapper::Get( info.FillMode );
	desc.DepthBias = info.DepthBias;
	desc.FrontCounterClockwise = info.IsClockwise ? FALSE : TRUE;
	desc.ScissorEnable = info.EnableScissor;
	desc.DepthClipEnable = info.EnableZClipping;
	desc.SlopeScaledDepthBias = 0.0f;
	desc.DepthBiasClamp = 0.0f;
	desc.MultisampleEnable = false;
	desc.AntialiasedLineEnable = false;

	ComPtr< ID3D11RasterizerState > state;
	auto result = device->CreateRasterizerState( &desc, state.GetAddressOf() );
	if( FAILED( result ) )
		return nullptr;

	return new DX11RasterizerState( state, info );
}



//====================================================================================//
//			DepthStencilState	
//====================================================================================//

DX11DepthStencilState::DX11DepthStencilState( ComPtr< ID3D11DepthStencilState > state, const DepthStencilInfo& info )
	:	m_state( state )
	,	m_info( info )
{ }

// ================================ //
//
std::string						DX11DepthStencilState::GetResourceName() const
{
	return std::string();
}

// ================================ //
//
const DepthStencilInfo&			DX11DepthStencilState::GetDescriptor()
{
	return m_info;
}

// ================================ //
//
DX11DepthStencilState*			DX11DepthStencilState::Create			( const DepthStencilInfo& info )
{
	D3D11_DEPTH_STENCIL_DESC desc;
	desc.DepthEnable = info.EnableDepthTest;
	desc.StencilEnable = info.EnableStencilTest;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	ComPtr< ID3D11DepthStencilState > state;
	auto result = device->CreateDepthStencilState( &desc, state.GetAddressOf() );
	if( FAILED( result ) )
		return nullptr;

	return new DX11DepthStencilState( state, info );
}



//====================================================================================//
//			BlendingState	
//====================================================================================//

DX11BlendingState::DX11BlendingState( ComPtr< ID3D11BlendState > state, const BlendingInfo& info )
	:	m_state( state )
	,	m_info( info )
{}

// ================================ //
//
std::string						DX11BlendingState::GetResourceName() const
{
	return std::string();
}

// ================================ //
//
const BlendingInfo&				DX11BlendingState::GetDescriptor()
{
	return m_info;
}

DX11BlendingState*				DX11BlendingState::Create		( const BlendingInfo & info )
{
	D3D11_BLEND_DESC desc;
	desc.IndependentBlendEnable = false;
	desc.AlphaToCoverageEnable = false;
	desc.RenderTarget[ 0 ].BlendEnable = info.EnableBlending;
	desc.RenderTarget[ 0 ].BlendOp = DX11ConstantsMapper::Get( info.ColorOperation );
	desc.RenderTarget[ 0 ].BlendOpAlpha = DX11ConstantsMapper::Get( info.AlphaOperation );

	desc.RenderTarget[ 0 ].SrcBlend = DX11ConstantsMapper::Get( info.SrcColorBlend );
	desc.RenderTarget[ 0 ].DestBlend = DX11ConstantsMapper::Get( info.DstColorBlend );

	desc.RenderTarget[ 0 ].SrcBlendAlpha = DX11ConstantsMapper::Get( info.SrcAlphaBlend );
	desc.RenderTarget[ 0 ].SrcBlendAlpha = DX11ConstantsMapper::Get( info.DstAlphaBlend );
	desc.RenderTarget[ 0 ].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	ComPtr< ID3D11BlendState > state;
	auto result = device->CreateBlendState( &desc, state.GetAddressOf() );
	if( FAILED( result ) )
		return nullptr;

	return new DX11BlendingState( state, info );
}
