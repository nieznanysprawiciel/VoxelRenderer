#include "FakeTimeline.h"




FakeTimeline::FakeTimeline()
	:	TimelineBase( nullptr )
{ }

void FakeTimeline::SetCurrentTIme( TimeType time )
{
	m_currentTime = time;
}

void FakeTimeline::Update()
{ }

void FakeTimeline::Start()
{ }

void FakeTimeline::Stop()
{ }

void FakeTimeline::Pause()
{ }

void FakeTimeline::SetTimeline( std::shared_ptr<TimelineBase>& parent )
{ }

bool FakeTimeline::IsPaused()
{
	return false;
}
