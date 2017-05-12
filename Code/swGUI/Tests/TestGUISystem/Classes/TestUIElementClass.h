#pragma once


#include "swGUI/Core/Controls/UIElement.h"
#include "swGUI/Core/System/Events/IEventArgs.h"


#include <vector>

//using namespace sw::gui;




// ================================ //
//
struct ValidationEventArgs : public sw::gui::IEventArgs
{
	RTTR_ENABLE( sw::gui::IEventArgs );
};
DEFINE_OPTR_TYPE( ValidationEventArgs );


// ================================ //
//
struct UnusedEventArgs : public sw::gui::IEventArgs
{
	RTTR_ENABLE( sw::gui::IEventArgs );
};
DEFINE_OPTR_TYPE( UnusedEventArgs );


//====================================================================================//
//			Header	
//====================================================================================//

class TestUIElementClass : public sw::gui::UIElement
{
	RTTR_ENABLE( sw::gui::UIElement );
private:

	std::vector< sw::gui::UIElementOPtr >		m_children;

public:
	// Event part
	REGISTER_EVENT_DECLARATION( ValidationStarted, RoutingStrategy::Direct, TestUIElementClass, ValidationEventArgs );
	REGISTER_EVENT_DECLARATION( ValidationEnded, RoutingStrategy::Direct, TestUIElementClass, ValidationEventArgs );
	REGISTER_EVENT_DECLARATION( UnusedEvent, RoutingStrategy::Direct, TestUIElementClass, UnusedEventArgs );

	REGISTER_EVENT_DECLARATION( PreviewTunnelEvent, RoutingStrategy::Tunnel, TestUIElementClass, ValidationEventArgs );
	REGISTER_EVENT_DECLARATION( BubbleEvent, RoutingStrategy::Bubble, TestUIElementClass, ValidationEventArgs );
public:
	void		EventRaisingFunction		();
	void		RaiseTunnelEvent			();
	void		RaiseBubbleEvent			();

	/// Check functions.
	bool		ExistsDelegatesContainer	( sw::gui::EventType eventID );

#pragma region Inherited


	// Inherited via UIElement
	virtual bool HitTest( const sw::gui::Position & point ) override
	{
		return false;
	}

	virtual void OnRender( sw::gui::DrawingContext & context ) override
	{}

	virtual sw::gui::Size2D Measure( sw::gui::Size2D availableSize ) override
	{
		return sw::gui::Size2D();
	}

	virtual void Arrange( sw::gui::Rect & finalRect ) override
	{}

	virtual Size				GetNumChildren			() const override;
	virtual UIElement *			GetUIChild				( Size idx ) const override;
	virtual bool				AddChild				( sw::gui::UIElementOPtr && child ) override;

#pragma endregion




};
DEFINE_OPTR_TYPE( TestUIElementClass );

