#pragma once
/**
@file Visual.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/



#include "swGUI/Core/System/DataBinding/DependencyObject.h"
#include "swGUI/Core/System/CommonTypes/CommonTypes.h"
#include "swGUI/Core/System/CommonTypes/AccessKey.h"

#include "swGUI/Core/System/Rendering/DrawingContext.h"


namespace sw {
namespace gui
{


/**@brief Provide interface for visual controls which can be rendered.

Provides interfaces and base implementation of:
- HitTesting
- Rendering

This classs correpsonds to WPF Visual class.
@ingroup ControlsFramework*/
class Visual : public DependencyObject
{
	RTTR_ENABLE( DependencyObject );
	RTTR_REGISTRATION_FRIEND;
private:

	Position		m_position;		///< Control position. (@todo Relative to parent ???)
	Size2D			m_actualSize;	///< Size of control used for rendering. This size will be set after for example layout arrangment will be aplied.

protected:


public:
	explicit		Visual		() = default;
	~Visual		() = default;

/**@brief Checks if point is within this object.
@todo We must specify if point is in relative coordinates or absolut.*/
	virtual bool			HitTest				( const Position& point )		= 0;

	/**@brief Control rendering behavior.*/
	virtual void			OnRender			( DrawingContext& context )		= 0;


};



}	// gui
}	// sw


