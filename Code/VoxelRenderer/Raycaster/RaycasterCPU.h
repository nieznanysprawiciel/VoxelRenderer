#pragma once


#include "IRaycaster.h"

#include "swCommonLib/Common/Multithreading/ThreadsBarrier.h"

#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

#include <stack>



namespace vr
{


/**@brief Bit shifts for specific position in child mask in OctreeNode.*/
typedef uint8 ChildFlag;
typedef uint8 StepDirection;


// ================================ //
//
struct ThreadData
{
	OctreePtr			Octree;
	CameraActor*		Camera;
	uint32*				Buffer;
	uint32				StartRange;
	uint32				EndRange;
};


// ================================ //
//
struct RaycasterContext
{
	OctreePtr						Octree;
	DirectX::XMFLOAT3				RayDirection;
	float							GridSize;		///< Grid size on current tree level. It changes during tree traversal.

	DirectX::XMFLOAT3				tMax;
	DirectX::XMFLOAT3				tDelta;

	uint8							StepXDir;
	uint8							StepYDir;
	uint8							StepZDir;

	std::stack< uint32 >			NodesStack;		///< Absolut offsets from beginning of array.
	uint32							Current;		///< Current node, we are in.
};


/**@brief CPU multithreaded implementation of raycaster.*/
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
	void			ReallocateRenderTexture	( uint16 newWidth, uint16 newHeight );

	void			UpdateRenderTarget		( uint32* buffer, RenderTargetObject* svoRenderTarget );
	void			SpawnThreads			( OctreePtr octree, CameraActor* camera );

	void			RaycasterThread			( Size threadNumber );
	void			RaycasterThreadImpl		( ThreadData& data, Size threadNumber );

	void			PrepareThreads			();
	uint16			GetNumThreads			() const;

private:
	
	// Raycasting core
	DirectX::XMFLOAT3		ComputeRayPosition		( CameraActor* camera, int screenX, int screenY );
	DirectX::XMFLOAT3		ComputeRayDirection		( CameraActor* camera, int screenX, int screenY );
	const OctreeNode&		FindStartingNode		( const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& direction, RaycasterContext& raycasterContext );

	// Step along axis and find new node.
	bool					Step					( RaycasterContext& raycasterContext, StepDirection stepAxis );
	void					StepUp					( RaycasterContext& raycasterContext );
	void					StepDown				( RaycasterContext& raycasterContext );

	// Attributes
	const OctreeLeaf&		GetResultLeafNode		( RaycasterContext& raycasterContext ) const;
	const VoxelAttributes&	GetLeafAttributes		( const OctreeLeaf& leaf, RaycasterContext& raycasterContext ) const;
	const BlockDescriptor&	GetBlockDescriptor		( const OctreeLeaf& leaf, RaycasterContext& raycasterContext ) const;


	bool					IsEmpty					( const OctreeNode& node );
	bool					IsRayOutside			( ChildFlag childFlag );
	ChildFlag				ComputeNextChildFlag	( ChildFlag curFlag, StepDirection stepAxis );
	ChildFlag				ComputeNodeFlag			( uint32 parent, uint32 current, OctreePtr& octree );
	ChildFlag				FindNodeFlag			( uint8 childMask, uint8 nodeNum );
	const OctreeNode&		SetCurrentNode			( uint32 parent, ChildFlag newChild, RaycasterContext& raycasterContext );

	uint8					CountNodesBefore		( ChildFlag childFlag, uint8 childMask );
};


}

