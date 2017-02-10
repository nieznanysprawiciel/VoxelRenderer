#pragma once
/**
@file TimelineBase.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "swCommonLib/Common/EngineObject.h"
#include "swCommonLib/ParameterAnimation/Timelines/TimelineTypes.h"

#include <memory>

/**@defgroup Timelines Timelines
@ingroup KeyFrameAnimation*/


class TimelineBase;
DEFINE_PTR_TYPE( TimelineBase );
DEFINE_WPTR_TYPE( TimelineBase );



/**@brief Base class for timelines.

@ingroup Timelines*/
class TimelineBase : public EngineObject
{
private:
protected:

	TimeType				m_currentTime;		///< Evaluated timeline time. Time is evaluated once.
	TimelineBaseWPtr		m_parent;

public:
	explicit		TimelineBase	() = default;
	explicit		TimelineBase	( TimelineBasePtr parent );
	virtual			~TimelineBase	() = default;


	
	virtual void		Update		()								= 0;
	virtual void		Start		()								= 0;
	virtual void		Stop		()								= 0;
	virtual void		Pause		()								= 0;

	virtual void		SetTimeline	( TimelineBasePtr& parent )		= 0;

	virtual bool		IsPaused	()								= 0;

	/**@brief Current time.
	@return Returns current time evaluated by this timeline.*/
	inline TimeType		GetTime	()			{ return m_currentTime; };
};


