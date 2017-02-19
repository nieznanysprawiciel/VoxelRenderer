#pragma once
/**
@file DependencyObject.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swCommonLib/Common/EngineObject.h"


namespace sw {
namespace gui
{

/**@brief Support for data binding.

It should correpsond WPF DependencyObject.*/
class DependencyObject : public EngineObject
{
	RTTR_ENABLE( EngineObject )
		RTTR_REGISTRATION_FRIEND
private:
protected:

	EngineObject*			m_dataContext;		///< Context for binding. @todo Consider using std::shared_ptr.


public:
	explicit		DependencyObject	();
	~DependencyObject	() = default;

};



}	// gui
}	// sw

