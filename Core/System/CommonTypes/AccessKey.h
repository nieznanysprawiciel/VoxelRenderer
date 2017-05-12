#pragma once
/**
@file AccessKey.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/


namespace sw {
namespace gui
{

/**@brief Class Restincts access to some functions only for chosen classes.*/
class AccessKey
{
	friend class GUISystem;
	friend class EventsSystem;
private:
	AccessKey() = default;					///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
	AccessKey( const AccessKey& ) {}		///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
	AccessKey( const AccessKey&& ) {}		///<Tylko klasa zaprzyjaŸniona mo¿e stworzyæ obiekt.
public:
};

}	// gui
}	// sw