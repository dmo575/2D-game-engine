#pragma once
#include "Component.h"
#include "EventSystem.h"
#include <memory>


class UC_EventSenderTest : public Component, public IEventSender
{
public:
    UC_EventSenderTest();
    virtual ~UC_EventSenderTest() override;

    //  IEventSender
    virtual void Sub(Event e, IEventListener* listener) override;
    virtual void Unsub(Event e, IEventListener* listener)override;

private:
    //  Component
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    std::unique_ptr<OEventSender> m_pEventSender;
};