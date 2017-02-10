#pragma once
/**
@file TimelineBase.cpp
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "TimelineBase.h"


// ================================ //
//
TimelineBase::TimelineBase( TimelineBasePtr parent )
	:	m_parent( parent )
	,	m_currentTime( 0.0 )
{}

