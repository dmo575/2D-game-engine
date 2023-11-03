#include "UC_ShieldTrigger.h"
#include "UC_Includes.h"

UC_ShieldTrigger::UC_ShieldTrigger(Point transformPos, Rect colRect)
    : m_pCol(nullptr)
    , m_transformPos(transformPos)
    , m_colRect(colRect)
{}

UC_ShieldTrigger::~UC_ShieldTrigger()
{}

void UC_ShieldTrigger::Init()
{
    m_pEventSender = std::make_unique<OEventSender>(m_pParent);

    auto transform = m_pParent->AddComponent(new EC_Transform);
    m_pCol = m_pParent->AddComponent(new EC_Collider);

    transform->SetPos(m_transformPos);

    m_pCol->SetColRect(m_colRect);
    m_pCol->SetTrigger(true);
}

void UC_ShieldTrigger::Update(float delta)
{}

void UC_ShieldTrigger::OnHit(float dmg)
{
    m_pEventSender->TriggerEvent(Event::SHOOTER_SHIELD_DAMAGED);
}

void UC_ShieldTrigger::Sub(Event e, IEventListener* listener)
{
    m_pEventSender->Sub(e, listener);
}

void UC_ShieldTrigger::Unsub(Event e, IEventListener* listener)
{
    m_pEventSender->Unsub(e, listener);
}

void UC_ShieldTrigger::Enable(bool value)
{
    m_pCol->SetActive(value);
}