/**
@file RenderingSystem.cpp
@author nieznanysprawiciel
@copyright File is part of Sleeping Wombat Libraries.
*/

#include "RenderingSystem.h"



namespace sw {
namespace gui
{



RenderingSystem::RenderingSystem			( ResourceManager* resourceManager, IRendererOPtr&& renderer )
	:	m_resourceManager( resourceManager )
	,	m_renderer( std::move( renderer ) )
{}


}	// gui
}	// sw