#pragma once
#include "Component.h"
#include "EventSystem.h"

class UC_LoadListener : public Component, IEventListener
{
public:
    UC_LoadListener(const char* level, IEventSender* sender, bool justQuit = false);
    virtual ~UC_LoadListener() override;

    virtual void Trigger(Event e, Entity* source) override;

private:
    virtual void Init() override;

private:
    IEventSender* m_pSender;
    const char* m_levelToLoad;
    bool m_justQuit;
};