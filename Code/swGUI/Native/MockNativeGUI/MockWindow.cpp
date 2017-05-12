/**
@file MockWindow.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


#include "MockWindow.h"

#include "MockGUI.h"
#include "swCommonLib/Common/Converters.h"


namespace sw {
namespace gui
{



// ================================ //
//
MockWindow::MockWindow()
{ }


// ================================ //
//
MockWindow::~MockWindow()
{ }


/**@brief */
MockWindow*		MockWindow::CreateWindowInstance	( NativeWindowDescriptor& descriptor )
{
	MockWindow* nativeWindow = new MockWindow();
	if( !nativeWindow->Initialize( descriptor ) )
	{
		delete nativeWindow;
		return nullptr;
	}

	return nativeWindow;
}

/**@brief */
bool		MockWindow::Initialize	( NativeWindowDescriptor& descriptor )
{
	m_title = Convert::FromString< std::wstring >( descriptor.WindowTitle, L"Remember about title" );

	m_width = descriptor.Width;
	m_height = descriptor.Height;
	
	static Size handle = 0;
	m_windowHandle = (WindowHandle)++handle;

	return true;
}

// ================================ //
//
uint16			MockWindow::GetWidth() const
{
	return m_width;
}

// ================================ //
//
uint16			MockWindow::GetHeight() const
{
	return m_height;
}

// ================================ //
//
WindowHandler	MockWindow::GetHandle() const
{
	return (WindowHandler)m_windowHandle;
}

// ================================ //
//
std::string		MockWindow::GetTitle() const
{
	return Convert::ToString( m_title );
}

// ================================ //
//
void			MockWindow::Show()
{
	ShowAppWindow();
}

// ================================ //
//
void			MockWindow::Hide()
{
	HideAppWindow();
}

// ================================ //
//
void			MockWindow::SetTitle		( const std::string& newTitle )
{
	m_title = Convert::FromString< std::wstring >( newTitle, L"" );
}

// ================================ //
//
uint16			MockWindow::GetClientWidth() const
{
	return m_width;
}

// ================================ //
//
uint16			MockWindow::GetClientHeight() const
{
	return m_height;
}



/**@brief */
void		MockWindow::ShowAppWindow()
{}

/**@brief */
void		MockWindow::HideAppWindow()
{}




}	// gui
}	// sw

