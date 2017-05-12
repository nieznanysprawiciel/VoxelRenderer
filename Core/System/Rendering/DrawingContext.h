#pragma once
/**
@file DrawContext.h
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "swGUI/Core/Media/Brushes/Brush.h"
#include "swGUI/Core/System/CommonTypes/CommonTypes.h"


namespace sw {
namespace gui
{


/**@brief Context passed to rendering functions.

This context provides methods for rendering basic primitives like ellipses, rectangles, lines.

@ingroup RenderingSystem*/
class DrawingContext
{
private:
protected:
public:
	explicit		DrawingContext		() = default;
	~DrawingContext		() = default;


	///@name Simple shapes rendering interface
	///@{

	/**@brief Draws simple rectangle.*/
	void			DrawRectangle			( const Rect& position, Brush* background );
	/**@brief Draws simple rectangle with border.*/
	void			DrawRectangle			( const Rect& position, Brush* background, Brush* border, const DirectX::XMFLOAT4& borderWidth );
	/**@brief Draws rectangle with rounded corenders.*/
	void			DrawRoundedRectangle	( const Rect& position, Brush* background, Brush* border, const DirectX::XMFLOAT4& borderWidth, const DirectX::XMFLOAT4& corners );

	void			DrawEllipse				( const Rect& position, Brush* background );
	void			DrawEllipse				( const Rect& position, Brush* background, Brush* border, float borderWidth );


	///@}

};



}	// gui
}	// sw
