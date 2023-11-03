#pragma once
#include "Component.h"
#include "EventSystem.h"
#include "IShooterHittable.h"
#include <memory>

class EC_Collider;

class UC_ShieldTrigger : public Component, public IEventSender, public IShooterHittable
{
public:
    UC_ShieldTrigger(Point transformPos, Rect triggerWorldRect);
    virtual ~UC_ShieldTrigger() override;
    void Enable(bool value);
    virtual void OnHit(float dmg) override;
    virtual void Sub(Event e, IEventListener* listener) override;
    virtual void Unsub(Event e, IEventListener* listener) override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Collider* m_pCol;
    std::unique_ptr<OEventSender> m_pEventSender;
    Point m_transformPos;
    Rect m_colRect;
};