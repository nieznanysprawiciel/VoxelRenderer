#pragma once

#include "swCommonLib/ParameterAnimation/Timelines/TimelineBase.h"




/**@brief */
class FakeTimeline : public TimelineBase
{
private:
protected:
public:
	explicit		FakeTimeline	();
	virtual			~FakeTimeline	() = default;

	void			SetCurrentTIme	( TimeType time );


	// Inherited via TimelineBase
	virtual void Update() override;
	virtual void Start() override;
	virtual void Stop() override;
	virtual void Pause() override;
	virtual void SetTimeline( TimelineBasePtr& parent ) override;
	virtual bool IsPaused() override;

};

DEFINE_PTR_TYPE( FakeTimeline )
DEFINE_WPTR_TYPE( FakeTimeline )
