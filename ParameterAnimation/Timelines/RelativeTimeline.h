#pragma once
/**
@file RelativeTimeline.h
@author nieznanysprawiciel
@copyright File is part of graphic engine SWEngine.
*/

#include "TimelineBase.h"
#include "TimelineTypes.h"


/**@brief */
class RelativeTimeline : public TimelineBase
{
private:
protected:
	TimeType			m_offset;			///< Offset relative to parent.
	TimeType			m_duration;			///< Time after which timeline will be wrapped.
	float				m_factor;			///< Enables slow motion.
	WrapMode			m_preWrap;
	WrapMode			m_postWrap;

	bool				m_isStarted;
	bool				m_isPaused;

public:
	explicit		RelativeTimeline	();
	explicit		RelativeTimeline	( TimelineBasePtr parent );
	virtual			~RelativeTimeline	() = default;

	void			SetPreWrap	( WrapMode mode );
	void			SetPostWrap	( WrapMode mode );
	void			SetDuration	( TimeType duration );

	WrapMode		GetPreWrap	()	{ return m_preWrap; }
	WrapMode		GetPostWrap	()	{ return m_postWrap; }
	TimeType		GetDuration	()	{ return m_duration; }

	// Inherited via TimelineBase
	virtual void	Update		() override;
	virtual void	Start		() override;
	virtual void	Stop		() override;
	virtual void	Pause		() override;

	virtual void	SetTimeline	( TimelineBasePtr& parent ) override;
	virtual bool	IsPaused	() override;

protected:
	TimeType		EvalTime			( TimeType parentTime );
	TimeType		EvalElapsedTime		( TimeType parentTime );
};

DEFINE_PTR_TYPE( RelativeTimeline );
DEFINE_WPTR_TYPE( RelativeTimeline );


