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
struct StackElement
{
	uint32		Node;
	float		tMax;

// ================================ //
//
	StackElement()
		:	Node( 0 )
		,	tMax( 0 )
	{}

	StackElement( uint32 node, float tMax )
		:	Node( node )
		,	tMax( tMax )
	{}
};


// ================================ //
//
struct RaycasterContext
{
	OctreePtr						Octree;
	DirectX::XMFLOAT3				RayDirection;
	DirectX::XMFLOAT3				RayStartPosition;
	float							GridSize;		///< Grid size on current tree level. It changes during tree traversal.

	const OctreeNode*				ChildDescriptor;

	//
	DirectX::XMFLOAT3				tCoeff;
	DirectX::XMFLOAT3				tBias;
	DirectX::XMFLOAT3				Position;
	float							tMax;
	float							tMin;

	uint32							Current;		///< Current node, we are in.
	ChildFlag						OctantMask;		///< Child bit flipping.
	ChildFlag						ChildIdx;		///< Child in children mask.
	std::vector< StackElement >		NodesStack;		///< Absolut offsets from beginning of array.

	int								Scale;
	float							ScaleExp;

	float							Depth;
	float							tCubeMin;
};


/**@brief CPU multithreaded implementation of raycaster.*/
class RaycasterCPU : public IRaycaster
{
protected:
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
	virtual void	ProcessInput		( const sw::input::MouseState& mouse, const sw::input::KeyboardState& keyboard )	override {}

private:

	void			ReallocateRenderBuffer	( uint16 newWidth, uint16 newHeight );
	void			ReallocateRenderTexture	( uint16 newWidth, uint16 newHeight );

	void			UpdateRenderTarget		( uint32* buffer, RenderTargetObject* svoRenderTarget );
	void			SpawnThreads			( OctreePtr octree, CameraActor* camera );

	void			RaycasterThread			( Size threadNumber );
	virtual void	RaycasterThreadImpl		( ThreadData& data, Size threadNumber );

	void			PrepareThreads			();
	uint16			GetNumThreads			() const;

protected:
	
	// Raycasting core
	DirectX::XMFLOAT3		ComputeRayPosition		( CameraActor* camera, int screenX, int screenY );
	DirectX::XMFLOAT3		ComputeRayDirection		( CameraActor* camera, int screenX, int screenY );
	void					InitRaycasting			( const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& direction, RaycasterContext& RaycasterContext );
	void					CastRay					( RaycasterContext& rayCtx );

	ChildFlag				AdvanceStep				( RaycasterContext& rayCtx, const DirectX::XMFLOAT3& corner, float tLeave );
	void					PopStep					( RaycasterContext& rayCtx, ChildFlag childIdxChange );
	void					PushStep				( RaycasterContext& rayCtx, const DirectX::XMFLOAT3& corner, float tVoxelMax, ChildFlag childShift );


	// Attributes
	const OctreeLeaf&		GetResultLeafNode		( RaycasterContext& raycasterContext ) const;
	const VoxelAttributes&	GetLeafAttributes		( const OctreeLeaf& leaf, RaycasterContext& raycasterContext ) const;
	const BlockDescriptor&	GetBlockDescriptor		( const OctreeLeaf& leaf, RaycasterContext& raycasterContext ) const;


	bool					IsEmpty					( const OctreeNode& node );
	bool					IsRayOutside			( ChildFlag childIdx, ChildFlag childIdxChange );
	uint8					CountNodesBefore		( ChildFlag childShift, uint8 childMask );

	DirectX::XMFLOAT3		ParamLine				( DirectX::XMFLOAT3& coords, RaycasterContext& raycasterContext );
	DirectX::XMFLOAT3		ParamLine				( DirectX::XMFLOAT3& coords, DirectX::XMFLOAT3& tCoeff, DirectX::XMFLOAT3& tBias );
	float					ParamLineX				( float posX, RaycasterContext& raycasterContext );
	float					ParamLineY				( float posY, RaycasterContext& raycasterContext );
	float					ParamLineZ				( float posZ, RaycasterContext& raycasterContext );

	float					Min						( DirectX::XMFLOAT3& coords );
	bool					ExistsChild				( const OctreeNode* node, ChildFlag childShift );
	bool					IsLeaf					( const OctreeNode* node );
	bool					IsIndirectPointer		( const OctreeNode* node );
	uint32					GetIndirectPtr			( RaycasterContext& rayCtx, const OctreeNode* node );
	uint32					ComputeChildOffset		( RaycasterContext& rayCtx, const OctreeNode* node, ChildFlag childShift );

	void					PushOnStack				( RaycasterContext& rayCtx, uint32 idx, uint32 node, float tMax );
	StackElement			ReadStack				( RaycasterContext& rayCtx, uint32 idx );

	static uint32			FindNewHierarchyLevel	( DirectX::XMFLOAT3& position, float scaleExp, ChildFlag childIdxChange );
};


}

