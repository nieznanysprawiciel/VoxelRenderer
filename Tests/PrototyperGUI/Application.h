#pragma once

#include "swGUI/Core/System/GUISystem.h"



/**@brief Application template class.

User should implement virtual functions to use GUI.*/
class Application : public sw::gui::GUISystem
{
private:
protected:
public:
	explicit	Application		( int argc, char** argv, sw::gui::INativeGUI* gui );
				~Application	() = default;

protected:
	virtual	bool	Initialize		() override;
	virtual bool	OnInitialized	() override;
	virtual void	OnClosing		() override;
	virtual void	OnIdle			( const sw::gui::FrameTime& frameTime ) override;

};

