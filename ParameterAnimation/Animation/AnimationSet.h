#pragma once
/**
@file AnimationSet.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "Common/ParameterAnimation/Animation/IAnimation.h"
#include "Common/ParameterAnimation/Timelines/TimelineBase.h"

#include <vector>


/**@brief 

@ingroup KeyFrameAnimation*/
class AnimationSet
{
private:
protected:
	std::vector< IAnimation* >			m_animations;
	std::vector< Ptr< TimelineBase > >	m_timelines;

public:
	explicit		AnimationSet() = default;
	~AnimationSet() = default;

};



