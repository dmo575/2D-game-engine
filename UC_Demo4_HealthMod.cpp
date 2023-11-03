#include "UC_Demo4_HealthMod.h"
#include "UC_Includes.h"
#include "UC_Demo4_Player.h"
#include "UC_WASDController.h"

UC_Demo4_HealthMod::UC_Demo4_HealthMod(Point size, int hpMod, bool isWinArea)
    : m_areaSize(size)
    , m_hpMod(hpMod)
    , m_pTransform(nullptr)
    , m_pCollider(nullptr)
    , m_pSound(nullptr)
    , m_playerInside(false)
    , m_pPlayerComp(nullptr)
    , m_pPlayerEnt(nullptr)
    , m_timeCounter(0.0f)
    , m_isWinArea(isWinArea)
{}

UC_Demo4_HealthMod::~UC_Demo4_HealthMod()
{}

void UC_Demo4_HealthMod::Init()
{
    Rect srcRect{ 0,0,64,64 };
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSound = m_pParent->AddComponent(new EC_Sound);
    auto sprite = m_pParent->AddComponent(new EC_Sprite);
    if(m_isWinArea)
        sprite->SetSourceImg("SP_WinZone.png", srcRect);
    else if (m_hpMod > 0)
    {
        sprite->SetSourceImg("SP_HealingZone.png", srcRect);
        m_pSound->SetSound("SN_HEAL.wav");
    }
    else
    {
        sprite->SetSourceImg("SP_DamageZone.png", srcRect);
        m_pSound->SetSound("SN_DMG.mp3");
    }

    m_pCollider = m_pParent->AddComponent(new EC_Collider);
    m_pCollider->AttachToSprite(true);
    m_pCollider->SetTrigger(true);
}

void UC_Demo4_HealthMod::Update(float delta)
{
    m_timeCounter += delta;

    if (m_timeCounter >= 1.0f)
    {
        if (m_pPlayerEnt && m_playerInside && !m_isWinArea)
        {
            m_pPlayerComp->AddHP(m_hpMod);
            m_pSound->Play();
        }

        m_timeCounter = 0.0f;
    }
}

void UC_Demo4_HealthMod::OnTriggerEnter(Entity* other, ColData data)
{
    if (other->IsBeingDestroyed()) return;

    if (other->GetName() == "Player")
    {
        if (m_pPlayerEnt == nullptr)
        {
           m_pPlayerEnt = other->GetSafeRef(&m_pPlayerEnt);
           m_pPlayerComp = other->GetComponent<UC_Demo4_Player>();
        }

        m_playerInside = true;
    }
}

void UC_Demo4_HealthMod::OnTriggerExit(Entity* other, ColData data)
{
    if(other->GetName() == "Player")
        m_playerInside = false;
}