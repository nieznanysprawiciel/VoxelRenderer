#pragma once

#include "swGUI/Core/System/GUISystem.h"

#include "Config.h"

#include "VoxelRenderer/Raycaster/IRaycaster.h"
#include "VoxelRenderer/Actors/CameraActor.h"

#include "VoxelRenderer/Application/TimeManager.h"

#include "Presentation/BlitEffect.h"

namespace vr
{



/**@brief Application template class.

User should implement virtual functions to use GUI.*/
class Application : public sw::gui::GUISystem
{
private:

	ConfigUPtr			m_config;
	
	IRaycasterUPtr		m_raycaster;
	CameraActor*		m_camera;
	OctreePtr			m_octree;

	ResourcePtr< RenderTargetObject >		m_svoRT;
	ResourcePtr< RenderTargetObject >		m_mainRT;

	BlitEffectUPtr		m_blitEffect;

	TimeManager			m_timeManager;

protected:
public:
	explicit	Application		( int argc, char** argv, sw::gui::INativeGUI* gui );
				~Application	() = default;

protected:
	virtual	bool	Initialize		() override;
	virtual bool	OnInitialized	() override;
	virtual void	OnClosing		() override;
	virtual void	OnIdle			( const sw::gui::FrameTime& frameTime ) override;


private:
	void			Update		( const sw::gui::FrameTime& frameTime );
	void			Render		( const sw::gui::FrameTime& frameTime );


private:
	///@name Initialization functions
	///@{
	void			InitCamera		();
	void			InitRaycaster	();
	void			InitOctree		();
	void			InitResources	();
	///@}
};


}	// vr
