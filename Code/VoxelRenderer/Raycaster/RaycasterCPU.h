#pragma once


#include "IRaycaster.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>


namespace vr
{




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
	std::mutex						m_lock;
	std::condition_variable_any		m_condition;
	std::atomic< bool >				m_run;
	std::atomic< bool >				m_end;


protected:
public:
	explicit		RaycasterCPU		();
					~RaycasterCPU		() = default;


	virtual void	Render				( OctreePtr octree, RenderTargetObject* svoRenderTarget, CameraActor* camera )		override;
	virtual void	Init				( IRenderer* renderer, ResourceManager* resourceManager )							override;


private:
	void			ReallocateRenderBuffer	( uint16 newWidth, uint16 newHeight );
	void			UpdateRenderTarget		( uint32* buffer, RenderTargetObject* svoRenderTarget );
	void			SpawnThreads			( OctreePtr octree, CameraActor* camera );
	void			ThreadWait				();

	void			RaycasterThread			( OctreePtr octree, CameraActor* camera, uint32 startRange, uint32 endRange );

	void			PrepareThreads			();
};


}

