#pragma once


#include "swGUI/Core/System/HostWindow.h"
#include "swInputLibrary/InputCore/Debugging/DebugInput.h"


/**@brief */
class HostWindowTester
{
private:
protected:
public:
	explicit		HostWindowTester		() = default;
	~HostWindowTester	() = default;


	sw::gui::HostWindowOPtr		CreateWindow	();
};


