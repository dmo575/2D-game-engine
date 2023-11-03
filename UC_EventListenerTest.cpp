#include "UC_EventListenerTest.h"
#include "UC_Includes.h"

UC_EventListenerTest::UC_EventListenerTest(IEventSender* sender)
	:m_pSender(sender)
{}

UC_EventListenerTest::~UC_EventListenerTest()
{}

void UC_EventListenerTest::Init()
{
	m_pSender->Sub(Event::CLICKABLE_HOVER_OUT, this);
	m_pSender->Sub(Event::CLICKABLE_HOVER_IN, this);
	m_pSender->Sub(Event::CLICKABLE_CLICK_DOWN, this);
	m_pSender->Sub(Event::CLICKABLE_CLICK_UP, this);
}

void UC_EventListenerTest::Update(float delta)
{}

void UC_EventListenerTest::Trigger(Event e, Entity* source)
{
	switch (e)
	{
	case Event::CLICKABLE_HOVER_IN:
		Logger::Log(std::string("HOVER_IN. (" + source->GetName() + ")").c_str());
		break;
	case Event::CLICKABLE_HOVER_OUT:
		Logger::Log(std::string("HOVER_OUT. (" + source->GetName() + ")").c_str());
		break;
	case Event::CLICKABLE_CLICK_DOWN:
		Logger::Log(std::string("CLICK_DOWN. (" + source->GetName() + ")").c_str());
		break;
	case Event::CLICKABLE_CLICK_UP:
		Logger::Log(std::string("CLICK_UP. (" + source->GetName() + ")").c_str());
		break;
	default:
		break;
	}
}