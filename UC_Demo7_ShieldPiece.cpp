#include "UC_Demo7_ShieldPiece.h"
#include "UC_Includes.h"
#include <iostream>

UC_Demo7_ShieldPiece::UC_Demo7_ShieldPiece(Point spawnPos)
    : m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_shieldImg("Shooter/SP_Shield.png")
    , m_srcRect(0,0,64,96)
    , m_currState(ShieldPieceState::k_Idle)
    , m_destPos(spawnPos)
    , m_travelStartPos(spawnPos)
    , m_spawnPos(spawnPos)
    , m_travelMaxDistance(0.0f)
    , m_vel(2000.0f)
    , m_HP(2)
{}

UC_Demo7_ShieldPiece::~UC_Demo7_ShieldPiece()
{}

void UC_Demo7_ShieldPiece::Init()
{
    m_pEventSender = std::make_unique<OEventSender>(m_pParent);
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pTransform->SetScale(0.5f, 0.5f);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite);

    m_pSprite->SetSourceImg(m_shieldImg.c_str(), m_srcRect);
    m_pSprite->SetPivotToCenter();

    SetState(ShieldPieceState::k_OOB);
}

void UC_Demo7_ShieldPiece::Sub(Event e, IEventListener* listener)
{
    m_pEventSender->Sub(e, listener);
}

void UC_Demo7_ShieldPiece::Unsub(Event e, IEventListener* listener)
{
    m_pEventSender->Unsub(e, listener);
}

void UC_Demo7_ShieldPiece::Update(float delta)
{
    UpdateCurrentState(delta);
}

void UC_Demo7_ShieldPiece::MoveShieldTo(Point newPos)
{
    m_destPos = newPos;
    SetState(ShieldPieceState::k_Moving);
}

void UC_Demo7_ShieldPiece::DamagePiece()
{
    m_HP--;

    if (m_HP == 1)
    {
        Rect srcRect = m_srcRect;
        srcRect.y = 96;
        m_pSprite->SetSourceRect(srcRect);
    }
    else if(m_HP == 0)
        SetState(ShieldPieceState::k_Destroyed);
}

void UC_Demo7_ShieldPiece::GetPieceOut()
{
    SetState(ShieldPieceState::k_OOB);
}

UC_Demo7_ShieldPiece::ShieldPieceState UC_Demo7_ShieldPiece::GetState()
{
    return m_currState;
}


void UC_Demo7_ShieldPiece::SetState(ShieldPieceState newState)
{
    //De-Init
    //N/A

    m_currState = newState;

    //Init
    switch (m_currState)
    {
    case UC_Demo7_ShieldPiece::ShieldPieceState::k_Idle:
    {
        m_pEventSender->TriggerEvent(Event::SHOOTER_SHIELDPIECE_ARRIVED);
        break;
    }
    case UC_Demo7_ShieldPiece::ShieldPieceState::k_OOB:
    {
        m_pTransform->SetPos(m_spawnPos);
        break;
    }
    case UC_Demo7_ShieldPiece::ShieldPieceState::k_Moving:
    {
        m_travelStartPos = m_pTransform->GetPos();
        m_travelMaxDistance = MathModule::Distance(m_travelStartPos, m_destPos);
        break;
    }
    case UC_Demo7_ShieldPiece::ShieldPieceState::k_Destroyed:
    {
        Rect srcRect = m_srcRect;
        srcRect.y = 96*2;
        m_pSprite->SetSourceRect(srcRect);
        break;
    }
    }
}

void UC_Demo7_ShieldPiece::UpdateCurrentState(float delta)
{
    switch (m_currState)
    {
    case UC_Demo7_ShieldPiece::ShieldPieceState::k_Moving:
    {
        //move towards dest
        m_pTransform->AddPos(MathModule::GetDirection(m_travelStartPos, m_destPos) * m_vel * delta);

        //if arrived, go IDLE
        if (MathModule::Distance(m_pTransform->GetPos(), m_travelStartPos) >= MathModule::Distance(m_travelStartPos, m_destPos))
        {
            //arrived
            m_pTransform->SetPos(m_destPos);
            SetState(ShieldPieceState::k_Idle);
        }
        break;
    }
    }
}