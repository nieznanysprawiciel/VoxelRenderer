#pragma once
/**
@file MockGUI.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swGUI/Native/INativeGUI.h"




/**@defgroup MockGUI Fake GUI for tests.
@brief Fake native GUI classes for tests.
@ingroup NativeGUI*/


namespace sw {
namespace gui
{


/**@brief 
@ingroup MockGUI*/
class MockGUI : public INativeGUI
{
private:
	NativeGUIInitData				m_initData;

protected:
public:
	explicit			MockGUI	();
	virtual				~MockGUI	() = default;


// Inherited via INativeGUI
	virtual bool				MainLoop		( bool blockingMode )							override;
	virtual bool				Init			( const NativeGUIInitData& initData )			override;
	virtual sw::input::IInput*	UseNativeInput	()												override;
	virtual INativeWindow*		CreateWindow	( NativeWindowDescriptor& descriptor )			override;
	virtual void				EndApplication	()												override;

	static MockGUI*				Create			();

	

};

}	// GUI
}	// sw
