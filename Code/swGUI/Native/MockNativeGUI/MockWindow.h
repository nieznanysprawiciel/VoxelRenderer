#pragma once
/**
@file MockWindow.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "swGUI/Native/INativeWindow.h"




namespace sw {
namespace gui
{


/**@brief Fake window for tests.
@ingroup MockGUI*/
class MockWindow : public INativeWindow
{
private:

	std::wstring	m_title;

	WindowHandler	m_windowHandle;
	uint16			m_width;
	uint16			m_height;

public:
	explicit			MockWindow	();
	~MockWindow	();


	static MockWindow*			CreateWindowInstance	( NativeWindowDescriptor& descriptor );

private:
	void			ShowAppWindow			();
	void			HideAppWindow			();

	bool			Initialize				( NativeWindowDescriptor& descriptor );


public:
	// Inherited via INativeWindow
	virtual uint16				GetWidth		() const override;
	virtual uint16				GetHeight		() const override;
	virtual uint16				GetClientWidth	() const override;
	virtual uint16				GetClientHeight	() const override;

	virtual WindowHandler		GetHandle		() const override;

	virtual std::string			GetTitle		() const override;

	virtual void				Show			() override;
	virtual void				Hide			() override;

	virtual void				SetTitle		( const std::string & newTitle ) override;
};


}	// gui
}	// sw
