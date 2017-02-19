#pragma once


#include "INativeWindow.h"
#include "swCommonLib/External/FastDelegate/FastDelegate.h"


class IInput;

namespace sw {
namespace gui
{

typedef fastdelegate::FastDelegate2< INativeWindow*, bool > ChangedFocusDelegate;


/**@brief Provides data needed for native GUI system initialization.*/
struct NativeGUIInitData
{
	ChangedFocusDelegate		FocusChanged;
};


/**@brief Encasulates native GUI system.*/
class INativeGUI
{
private:
protected:
public:
	INativeGUI() = default;
	~INativeGUI() = default;


	/**@brief Invokes main loop of native GUI.
	@return Returns true if application should close.*/
	virtual bool			MainLoop		( bool blockingMode ) = 0;

	/**@brief Initializes native GUI.
	@return Returns false if initialization failed.*/
	virtual bool			Init			( const NativeGUIInitData& initData ) = 0;

	/**@brief Initalize native api for this gui system and return IInput object.
	GUI can use native input api for specific gui system or provide different
	library to deal directly with hardware. Classes derived from INativeGUI are reposible
	for updating native inputs in main loop. In case of using separate hardware input library
	INativeGUI does nothing.*/
	virtual IInput*			UseNativeInput	() = 0;

	/**@brief Creates native GUI window.*/
	virtual INativeWindow*	CreateWindow	( NativeWindowDescriptor& descriptor ) = 0;
};


}	// gui
}	// sw
