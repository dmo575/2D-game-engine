#pragma once
#include "Component.h"
#include "EventSystem.h"

class UC_EventListenerTest : public Component, public IEventListener
{
public:
    UC_EventListenerTest(IEventSender* sender);
    virtual ~UC_EventListenerTest() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void Trigger(Event e, Entity* source) override;

private:
    IEventSender* m_pSender;
};