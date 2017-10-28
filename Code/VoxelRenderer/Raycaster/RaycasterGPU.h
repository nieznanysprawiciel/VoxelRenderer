#pragma once


#include "IRaycaster.h"



namespace vr
{

/**@brief Bit shifts for specific position in child mask in OctreeNode.*/
typedef uint8 ChildFlag;



/**@brief GPU raycaster using compute shaders.*/
class RaycasterGPU : public IRaycaster
{
protected:

	IRenderer*				m_renderer;
	ResourceManager*		m_resourceManager;

	OctreePtr				m_lastOctree;

	ResourcePtr< BufferObject >		m_cameraBuffer;
	ResourcePtr< BufferObject >		m_octreeBuffer;
	ResourcePtr< TextureObject >	m_octreeTexBuff;

	ResourcePtr< VertexShader >		m_vertexShader;
	ResourcePtr< PixelShader >		m_pixelShader;

	ResourcePtr< BlendingState >		m_blendingState;
	ResourcePtr< RasterizerState >		m_rasterizerState;
	ResourcePtr< DepthStencilState >	m_depthStencilState;

	LightModulePtr					m_lights;

public:



	virtual void			Render			( TimeType time, OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )		override;
	virtual void			Init			( IRenderer* renderer, ResourceManager* resourceManager, LightModulePtr lights )			override;
	virtual void			ProcessInput	( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )		override;


private:

	void					UpdateOctree	( OctreePtr octree );
	void					UpdateCamera	( CameraActor* camera );
};


}	// vr

