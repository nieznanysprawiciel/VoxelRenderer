#pragma once
/**
@file GUISystemConfig.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


namespace sw {
namespace gui
{


/**@brief Configuration flags.*/
class GUISystemConfig
{
private:

public:

	bool			UseBlockingMode		: 1;		///< GUI will always wait for native events. Set to false if you want maximum FPS.
	bool			UseVSync			: 1;		///< Synchronize swaping render targets with screen frequency. Set to false if you want maximum FPS.
	bool			RedrawOnlyFocused	: 1;		///< Only focused window will be redrawn.

public:

	explicit		GUISystemConfig()
		:	UseBlockingMode( true )
		,	UseVSync( true )
		,	RedrawOnlyFocused( true )
	{}

};

}	// gui
}	// sw

