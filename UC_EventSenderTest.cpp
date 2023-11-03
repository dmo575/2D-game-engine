#include "UC_EventSenderTest.h"
#include "UC_Includes.h"

UC_EventSenderTest::UC_EventSenderTest()
{}

UC_EventSenderTest::~UC_EventSenderTest()
{}

void UC_EventSenderTest::Init()
{
    m_pEventSender = std::make_unique <OEventSender>(m_pParent);
}

void UC_EventSenderTest::Update(float delta)
{
    if (InputModule::Get()->GetKeyState(KeyCode::k_T) == KeyState::k_KS_Press)
        m_pEventSender->TriggerEvent(Event::CLICKABLE_CLICK_DOWN);
}

void UC_EventSenderTest::Sub(Event e, IEventListener* listener)
{
    m_pEventSender->Sub(e, listener);
}

void UC_EventSenderTest::Unsub(Event e, IEventListener* listener)
{
    m_pEventSender->Unsub(e, listener);
}