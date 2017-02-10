#pragma once
/**
@file IAnimation.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

/**@defgroup KeyFrameAnimation Keyframe Animation
Animation of rttr properties.
@ingroup CommonFiles*/


class TimelineBase;


/**@brief Base animation class.

Allows such operation like evaluating animations without knowing type.
@ingroup KeyFrameAnimation*/
class IAnimation
{
private:
protected:
public:
	explicit		IAnimation() = default;
	virtual			~IAnimation() = default;


	virtual void			Animate	( EngineObject* object, TimelineBase* timeline )		= 0;
	virtual void			Animate	( EngineObject* object, TimeType time )					= 0;
};

