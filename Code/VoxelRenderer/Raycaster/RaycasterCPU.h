#pragma once


#include "IRaycaster.h"

#include "swCommonLib/Common/Multithreading/ThreadsBarrier.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>


namespace vr
{


struct ThreadData
{
	OctreePtr			Octree;
	CameraActor*		Camera;
	uint32*				Buffer;
	uint32				StartRange;
	uint32				EndRange;
};



/**@brief */
class RaycasterCPU : public IRaycaster
{
private:
	IRenderer*				m_renderer;
	ResourceManager*		m_resourceManager;

	uint16					m_width;
	uint16					m_height;

	std::unique_ptr< uint32[] >		m_renderBuffer;

	std::vector< std::thread >		m_threadPool;
	std::vector< ThreadData >		m_threadData;

	sw::ThreadsBarrier				m_raycastEndBarrier;
	sw::ThreadsBarrier				m_raycastLoopBarrier;
	
	std::atomic< bool >				m_end;


protected:
public:
	explicit		RaycasterCPU		();
					~RaycasterCPU		();


	virtual void	Render				( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )		override;
	virtual void	Init				( IRenderer* renderer, ResourceManager* resourceManager )							override;


private:
	void			ReallocateRenderBuffer	( uint16 newWidth, uint16 newHeight );
	void			UpdateRenderTarget		( uint32* buffer, RenderTargetObject* svoRenderTarget );
	void			SpawnThreads			( OctreePtr octree, CameraActor* camera );

	void			RaycasterThread			( Size threadNumber );
	void			RaycasterThreadImpl		( ThreadData& data );

	void			PrepareThreads			();
	uint16			GetNumThreads			() const;
};


}

