#pragma once
/**
@file IRenderer.h
@author nieznanysprawiciel
@copyright Plik jest czêœci¹ silnika graficznego SWEngine.
*/


#include "swGraphicAPI/Resources/MeshResources.h"
#include "RenderCommands.h"


/**@brief Sposób u¿ycia renderera.

Renderer mo¿e wysy³aæ polecenia na kartê graficzn¹ od razu lub
zamiast tego tworzyc kolejkê komunikatów i wysy³aæ dopiero kiedy wszystko bêdzie przygotowane.
Ta druga opcja umo¿liwia renderowanie wielow¹tkowe.*/
enum RendererUsage
{
	USE_AS_DEFERRED,			///<Renderowanie do kolejki rozkazów.
	USE_AS_IMMEDIATE			///<Renderowanie natychmiastowe
};



/**@brief Klasa interfejsu renderera.
@ingroup GraphicAPI
@ingroup RenderingCommands*/
class IRenderer
{
private:
public:
	IRenderer() = default;
	virtual ~IRenderer() = default;


	virtual bool SupportsDefferedContextRendering() = 0;		///< Informuje czy Renderer nadaje siê do renderowania w wielu w¹tkach do opóŸnionego contextu.
	virtual bool IsValid() = 0;


	virtual void		Draw					( const DrawCommand& command )						= 0;
	virtual void		DrawInstanced			( const DrawInstancedCommand& command )				= 0;

	virtual void		SetRenderTarget			( const SetRenderTargetCommand& command )			= 0;
	virtual void		SetRenderTarget			( const SetRenderTargetExCommand& command )			= 0;
	virtual void		ClearRenderTarget		( const ClearRenderTargetCommand& command )			= 0;

	virtual void		UpdateBuffer			( const UpdateBufferCommand& command )				= 0;
	virtual void		BindBuffer				( const BindBufferCommand& command )				= 0;
	virtual void		UpdateAndBindBuffer		( const UpdateBindBuffer& command )					= 0;

	virtual void		SetDefaultBuffers		( const SetDefaultBuffersCommand& command )			= 0;
	
	virtual void		SetShaderState			( const SetShaderStateCommand& command )			= 0;
	virtual void		SetShaderState			( const SetShaderStateExCommand& command )			= 0;
	virtual void		SetShaderState			( const SetRenderStateCommand& command )			= 0;
	virtual void		SetShaderState			( const SetRenderStateExCommand& command )			= 0;

	virtual void		CopyTexture				( const CopyTextureCommand& command )				= 0;
};


