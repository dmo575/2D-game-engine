#include "UC_Demo4_Player.h"
#include "UC_Includes.h"
#include "UC_WASDController.h"
#include "UC_Demo4_HealthMod.h"
#include "UC_MT_MovGridLocker.h"
#include "UC_Demo4_HPHUD.h"

#include <iostream>

UC_Demo4_Player::UC_Demo4_Player(Point startPos)
    : m_pAnimator(nullptr)
    , m_pCollider(nullptr)
    , m_pSound(nullptr)
    , m_pSprite(nullptr)
    , m_pTransform(nullptr)
    , m_pController(nullptr)
    , m_pMovLocker(nullptr)
    , m_startPos(startPos)
    , m_lastMovingDir(0, 0)
    , m_currHP(7)
    , m_maxHP(7)
    , m_keyCounter(0)
    , Component()
{}

UC_Demo4_Player::~UC_Demo4_Player()
{}

void UC_Demo4_Player::Init()
{
    //Add components
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite(0U));
    m_pAnimator = m_pParent->AddComponent(new EC_Animator);
    m_pCollider = m_pParent->AddComponent(new EC_Collider);
    m_pController = m_pParent->AddComponent(new UC_WASDController(100.0f, UC_WASDController::DirType::FOUR));

    //Set up transform
    m_pTransform->SetPos(m_startPos);

    //Set up Sprite
    Rect rect64(0, 0, 64, 64);
    m_pSprite->SetSourceImg("SS_Knight_64x64.png", rect64);
    m_pSprite->SetPivotToCenter();

    //Set up Collider
    m_pCollider->AttachToSprite(false);
    m_pCollider->SetColRect(Rect(-12, -14, 24, 40));
    m_pCollider->SetTrigger(false);

    //Set up animator
    m_pAnimator->SetSpeed(0.15f);
    m_pAnimator->AddClip("WALK_NORTH", 8, Rect{64,0,64,64});
    m_pAnimator->AddClip("IDLE_NORTH", 1, Rect{ 0,0,64,64 });
    m_pAnimator->AddClip("WALK_SOUTH", 8, Rect{ 64,64*2,64,64 });
    m_pAnimator->AddClip("IDLE_SOUTH", 1, Rect{ 0,64 * 2,64,64 });
    m_pAnimator->AddClip("WALK_EAST", 8, Rect{ 64,64*3,64,64 });
    m_pAnimator->AddClip("IDLE_EAST", 1, Rect{ 0,64 * 3,64,64 });
    m_pAnimator->AddClip("WALK_WEST", 8, Rect{ 64,64,64,64 });
    m_pAnimator->AddClip("IDLE_WEST", 1, Rect{ 0,64,64,64 });

    m_pAnimator->Play(true);
    m_pAnimator->SetClip("IDLE_SOUTH");

    //Set up sound
    m_pSound = m_pParent->AddComponent(new EC_Sound);
    m_pSound->SetSound("SN_PICKUP.wav");


    auto hudEnt = m_entityManager->AddEntity("HUD");
    hudEnt->AddComponent(new UC_Demo4_HPHUD(m_pParent));
}

void UC_Demo4_Player::Update(float delta)
{
    UpdateAnimationClip();
}

void UC_Demo4_Player::UpdateAnimationClip()
{
    Point currDir = m_pController->GetCurrDir();

    if (currDir.x != 0 || currDir.y != 0)
    {
        if (currDir.x > 0 && m_pAnimator->GetClipName() != "WALK_EAST")
            m_pAnimator->SetClip("WALK_EAST");
        else if (currDir.x < 0)
            m_pAnimator->SetClip("WALK_WEST");
        else if (currDir.y > 0)
            m_pAnimator->SetClip("WALK_SOUTH");
        else if (currDir.y < 0)
            m_pAnimator->SetClip("WALK_NORTH");

        m_lastMovingDir = currDir;
    }
    else
    {
        if(m_lastMovingDir.x > 0)
            m_pAnimator->SetClip("IDLE_EAST");
        else if (m_lastMovingDir.x < 0)
            m_pAnimator->SetClip("IDLE_WEST");
        else if (m_lastMovingDir.y > 0)
            m_pAnimator->SetClip("IDLE_SOUTH");
        else if (m_lastMovingDir.y < 0)
            m_pAnimator->SetClip("IDLE_NORTH");
    }
}

void UC_Demo4_Player::OnTriggerStay(Entity* other, ColData data)
{
    if (other->IsBeingDestroyed()) return;
}

void UC_Demo4_Player::OnTriggerEnter(Entity* other, ColData data)
{
    if (!other) return;

    if (other->GetName() == "Key")
    {
        m_keyCounter++;
        other->Destroy();
        m_pSound->Play();
    }
    else if (other->GetName() == "Skeleton")
    {
        AddHP(-1, true);
    }
    else if (other->GetName() == "WinArea")
    {
        m_pController->SetActive(false);
        m_isActive = false;
        LevelManager::Get()->QueueLoadLevel("demo_7_ShooterTutorial");
    }
}

void UC_Demo4_Player::OnCollision(Entity* other, ColData data)
{
    if (other->GetName() == "Door" && m_keyCounter == 2)
        other->Destroy();
}


void UC_Demo4_Player::AddHP(int value, bool respawn)
{
    m_currHP += value;

    if (m_currHP > m_maxHP)
        m_currHP = m_maxHP;
    else if (m_currHP <= 0)
    {
        m_currHP = 0;
        m_pParent->Destroy();
        LevelManager::Get()->QueueLoadLevel("demo_6_GameOver");
    }
    else if(value < 0 && respawn)
        m_pTransform->SetPos(m_startPos);
}

int UC_Demo4_Player::GetHP()
{
    return m_currHP;
}

void UC_Demo4_Player::SetNewHP(int value)
{
    m_maxHP = value;
    m_currHP = value;
}