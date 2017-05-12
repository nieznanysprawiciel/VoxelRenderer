#pragma once

#include "swGraphicAPI/Resources/MeshResources.h"
#include "swGraphicAPI/Resources/SwapChain.h"

#include <string>



namespace sw {
namespace gui
{


/**@brief Dedcriptor used to create native GUI window.
@ingroup NativeGUI*/
struct NativeWindowDescriptor
{
	WindowHandler			ParentHandle;
	unsigned short			Width;
	unsigned short			Height;
	unsigned short			PositionX;
	unsigned short			PositionY;
	bool					Fullscreen : 1;
	bool					ShowWindow : 1;	///< Show window immediately after creation.
	bool					AddExitButton : 1;
	bool					AddMinimizeButton : 1;
	bool					AddMaximizeButton : 1;
	bool					AddFrame : 1;
	bool					AddTitleBar : 1;
	bool					AdjustSize : 1;	///< Adjust window size to client area.
	std::string				WindowTitle;


// ================================ //
//
	NativeWindowDescriptor()
	{
		InitDefaults();
	}

	NativeWindowDescriptor( const std::string& windowTitle )
		: WindowTitle( windowTitle )
	{
		InitDefaults();
	}

	void InitDefaults()
	{
		ParentHandle = nullptr;
		Width = 1024;
		Height = 768;
		PositionX = 0;
		PositionY = 0;
		Fullscreen = false;
		ShowWindow = true;
		AddExitButton = true;
		AddMinimizeButton = true;
		AddMaximizeButton = true;
		AddFrame = true;
		AddTitleBar = true;
		AdjustSize = true;
	}
};


/**@brief Interface for native window sing OS specific ffunctions.

Natywne okno powinno udostêpniaæ przynajmniej swój render target i swapchain.
Ponadto mo¿e zawieraæ funkcje do obs³ugi prawdziwego okna, ale nie jest to konieczne.
GUI nie musi samo obs³ugiwaæ prawdziwych okien, mo¿e mieæ je dostarczone przez aplikacjê.

@ingroup NativeGUI*/
class INativeWindow
{
private:
protected:
public:
	INativeWindow() = default;
	~INativeWindow() = default;

	virtual uint16				GetWidth	() const		= 0;
	virtual uint16				GetHeight	() const		= 0;

	virtual uint16				GetClientWidth	() const	= 0;
	virtual uint16				GetClientHeight	() const	= 0;

	virtual WindowHandler		GetHandle	() const		= 0;
	virtual std::string			GetTitle	() const		= 0;

	virtual void				Show		() = 0;
	virtual void				Hide		() = 0;

	virtual void				SetTitle	( const std::string& newTitle ) = 0;
};



}	// gui
}	// sw
