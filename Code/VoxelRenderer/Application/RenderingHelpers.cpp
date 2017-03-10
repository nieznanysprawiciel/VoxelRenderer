//#include "EngineCore/stdafx.h"
/**
@file RenderingHelpers.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "RenderingHelpers.h"

#include "swGraphicAPI/Rendering/IRenderer.h"


using namespace DirectX;



// ================================ //
//
void					RenderingHelper::ClearRenderTargetAndDepth		( RenderTargetObject* target, const DirectX::XMFLOAT4& color, float depth )
{
	ClearRenderTargetAndDepth( m_renderer, target, color, depth );
}

// ================================ //
//
void					RenderingHelper::SetRenderTarget				( RenderTargetObject* target,
																		  RasterizerState* rasterizerState,
																		  BlendingState* blendingState,
																		  DepthStencilState* depthStencilState )
{
	SetRenderTarget( m_renderer, target, rasterizerState, blendingState, depthStencilState );
}

// ================================ //
//
void					RenderingHelper::DrawBufferLess					( uint32 numVerticies, PrimitiveTopology topology )
{
	DrawBufferLess( m_renderer, numVerticies, topology );
}

// ================================ //
//
void					RenderingHelper::ClearRenderTargetAndDepth		( IRenderer* renderer, RenderTargetObject* target, const DirectX::XMFLOAT4& color, float depth )
{
	ClearRenderTargetCommand clearCommand;
	clearCommand.ClearDepth = true;
	clearCommand.ClearRenderTarget = true;
	clearCommand.ClearStencil = false;
	clearCommand.RenderTarget = target;
	clearCommand.DepthValue = depth;
	clearCommand.ClearColor = color;

	renderer->ClearRenderTarget( clearCommand );
}

// ================================ //
//
void					RenderingHelper::SetRenderTarget				( IRenderer* renderer,
																		  RenderTargetObject* target,
																		  RasterizerState* rasterizerState,
																		  BlendingState* blendingState,
																		  DepthStencilState* depthStencilState )
{
	SetRenderTargetCommand setRT;
	for( auto& rt : setRT.RenderTargets )
		rt = nullptr;

	setRT.RenderTargets[ 0 ] = target;
	setRT.CameraBuffer= nullptr;
	setRT.DepthStencil = target;
	setRT.LightBuffer = nullptr;
	setRT.BlendingState = blendingState;
	setRT.DepthStencilState = depthStencilState;
	setRT.RasterizerState = rasterizerState;

	renderer->SetRenderTarget( setRT );
}

// ================================ //
//
void					RenderingHelper::DrawBufferLess					( IRenderer* renderer, uint32 numVerticies, PrimitiveTopology topology )
{
	DrawCommand drawCmd;
	drawCmd.IndexBufer = nullptr;
	drawCmd.VertexBuffer = nullptr;
	drawCmd.ExtendedIndex = false;
	drawCmd.BaseVertex = 0;
	drawCmd.BufferOffset = 0;
	drawCmd.NumVertices = numVerticies;
	drawCmd.Layout = nullptr;
	drawCmd.Topology = topology;

	renderer->Draw( drawCmd );
}


