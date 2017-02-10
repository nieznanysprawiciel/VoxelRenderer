#pragma once
/**
@file RenderCommands.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swGraphicAPI/Resources/MeshResources.h"
#include "swGraphicAPI/Resources/BlendingState.h"
#include "swGraphicAPI/Resources/DepthStencilState.h"
#include "swGraphicAPI/Resources/RasterizerState.h"

/**@defgroup RenderingCommands Rendering commands
Commands for @ref IRenderer class.

@ingroup GraphicAPI*/


#define MAX_BOUND_RENDER_TARGETS    8
#define ENGINE_MAX_TEXTURES         8


/**@brief Base class for renderer commands.
@ingroup RenderingCommands*/
struct RendererCommand
{};


/**@brief Set state of rasterizer and render target.

Uses default viewport settings (width and hight from render target).
Disables scissors rectangle.
If you want your custom settings use @ref SetRenderTargetExCommand.

@ingroup RenderingCommands*/
struct SetRenderTargetCommand : public RendererCommand
{
	RenderTargetObject*		RenderTargets[ MAX_BOUND_RENDER_TARGETS ];	///< Render targets. If you want to use only one slot, set other to nullptr.
	RenderTargetObject*		DepthStencil;		///< Renderer will extract depth stencil from this render target. You can use one of objects from RenderTargets array.
	RasterizerState*		RasterizerState;
	BlendingState*			BlendingState;
	DepthStencilState*		DepthStencilState;
	BufferObject*			CameraBuffer;		///< Buffer updated once per render target (or even once per frame). Buffer is bound to both pixel and vertex shader.
	BufferObject*			LightBuffer;		///< Buffer updated once per render target (or even once per frame). Buffer is bound only to pixel shader.
};


/**@brief Viewport data.

@see SetRenderTargetExCommand
@ingroup RenderingCommands*/
struct ViewportInfo
{
	uint16		TopLeftX;
	uint16		TopLeftY;
	uint16		Width;
	uint16		Height;
	float		MinDepth;
	float		MaxDepth;
};


/**@brief ScissorRect data.

@see SetRenderTargetExCommand
@ingroup RenderingCommands*/
struct ScissorRectInfo
{
	uint16		Left;
	uint16		Top;
	uint16		Right;
	uint16		Bottom;
};


/**@brief Set state of rasterizer and render target.

Sets same number of scissors and viewports. Viewports and Scissors
arrays can be alocated on stack.

@ingroup RenderingCommands*/
struct SetRenderTargetExCommand : public SetRenderTargetCommand
{
	uint8					NumViews;
	ViewportInfo*			Viewports;
	ScissorRectInfo*		Scissors;
	DirectX::XMFLOAT4		BlendFactor;
	uint32					StencilMask;
};


/**@brief Set state of rasterizer and render target.
@ingroup RenderingCommands*/
struct ClearRenderTargetCommand : public RendererCommand
{
	RenderTargetObject*		RenderTarget;
	DirectX::XMFLOAT4		ClearColor;
	float					DepthValue;
	uint8					StencilValue;
	bool					ClearDepth;
	bool					ClearStencil;
	bool					ClearRenderTarget;
};

/**@brief Main draw command.

If mesh doesn't have index buffer, set IndexBuffer field to nullptr.
@ingroup RenderingCommands*/
struct DrawCommand : public RendererCommand
{
	BufferObject*			VertexBuffer;
	BufferObject*			IndexBufer;
	ShaderInputLayout*		Layout;
	uint32					BufferOffset;
	uint32					NumVertices;
	int32					BaseVertex;
	PrimitiveTopology		Topology;
	bool					ExtendedIndex;		///< Use 4 @ref Index32 instead of @ref Index16.
};

/**@brief Instanced drawing command.

@ingroup RenderingCommands*/
struct DrawInstancedCommand : public DrawCommand
{
	BufferObject*		PerInstanceBuffer;		///< Per instance transformation.
	uint16				NumInstances;
};


/**@brief Sets basic shaders and textures.

To set Geometry shader and Tesselation shaders use @ref SetShaderStateCommandEx.

@ingroup RenderingCommands*/
struct SetShaderStateCommand : public RendererCommand
{
	VertexShader*		VertexShader;
	PixelShader*		PixelShader;
	TextureObject*		Textures[ 8 ];
	uint8				BindToShader[ 8 ];		///< Use @ref ShaderType flag. @note ShaderType::ComputeShader will be ignored.
};


/**@brief Sets geometry and tesselation shaders.

@ingroup RenderingCommands*/
struct SetShaderStateExCommand : public SetShaderStateCommand
{
	GeometryShader*		GeometryShader;
	EvaluationShader*	TessEvaluationShader;
	ControlShader*		TessControlShader;
};


/**@brief Sets default buffers.

@ingroup RenderingCommands*/
struct SetDefaultBuffersCommand : public RendererCommand
{
	BufferObject*		TransformBuffer;
	BufferObject*		MaterialBuffer;
	BufferObject*		BonesTransforms;
};

/**@brief 

@ingroup RenderingCommands*/
struct SetRenderStateCommand : public SetShaderStateCommand, SetDefaultBuffersCommand
{};


/**@brief 

@ingroup RenderingCommands*/
struct SetRenderStateExCommand : public SetShaderStateExCommand, SetDefaultBuffersCommand
{};


/**@brief Copy texture

@ingroup RenderingCommands*/
struct CopyTextureCommand : public RendererCommand
{
	TextureObject*		SourceTexture;
	TextureObject*		DestinationTexture;
};

/**@brief Binds additional buffers.

Buffer slots (binding points) 0 - 2 are reserved for default engine constant buffers.
Calling SetRenderTargetCommand or SetShaderStateCommand (or derive) after invoking this command
will override this setting.

@ingroup RenderingCommands*/
struct BindBufferCommand : public RendererCommand
{
	BufferObject*		Buffer;
	uint8				BufferSlot;
	uint8				BindToShader;		///< Use @ref ShaderType flag. @note ShaderType::ComputeShader will be ignored.
};


/**@brief Updates buffer with given data.

@ingroup RenderingCommands*/
struct UpdateBufferCommand : public RendererCommand
{
	BufferObject*		Buffer;
	uint8*				FillData;
	uint32				Size;		///< Size in bytes.
};


/**@brief Binds buffer to pipeline and updates with given data.

@ingroup RenderingCommands*/
struct UpdateBindBuffer : public BindBufferCommand
{
	uint8*				FillData;
	uint32				Size;		///< Size in bytes.
};
