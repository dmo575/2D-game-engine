#include "UC_Demo4_Ghost.h"
#include "UC_Includes.h"
#include "UC_WASDController.h"
#include "UC_Demo4_HealthMod.h"

#include <string>

UC_Demo4_Ghost::UC_Demo4_Ghost(float vel, bool AddCol)
    : Component()
    , m_pCollider(nullptr)
    , m_pSprite(nullptr)
    , m_pTransform(nullptr)
    , m_pAnimator(nullptr)
    , randomObj(Random::GetObject())
    , m_startPos(0,0)
    , m_currDir(0, 1)
    , m_currDest(0,0)
    , m_vel(vel)
    , m_addCol(AddCol)
{}

UC_Demo4_Ghost::~UC_Demo4_Ghost()
{}

void UC_Demo4_Ghost::Init()
{
    //Add components
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite(1U));
    if(m_addCol)
        m_pCollider = m_pParent->AddComponent(new EC_Collider);
    m_pAnimator = m_pParent->AddComponent(new EC_Animator);

    //Set up transform
    m_startPos = randomObj->GetRandomPoint(0, 500);

    //This makes sure that the ghost will not spawn near the player at the beginning
    while(m_startPos.x <= 100 && m_startPos.y >= 450)
        m_startPos = randomObj->GetRandomPoint(0, 500);

    m_pTransform->SetPos(m_startPos);
    m_pTransform->SetScale(0.5f, 0.5f);

    //Set up Sprite
    Rect rect64(0, 0, 64, 64);
    m_pSprite->SetSourceImg("SS_Ghost.png", rect64);
    m_pSprite->SetPivotToCenter();

    //Set up Collider
    if (m_addCol)
    {
        m_pCollider->AttachToSprite(false);
        m_pCollider->SetColRect(Rect(-12, -14, 24, 25));
        m_pCollider->SetTrigger(true);//We use them as triggers so we can overlap enemies crossing pahts
    }

    //Set up animator
    m_pAnimator->AddClip("RIGHT", 1, rect64);
    m_pAnimator->AddClip("LEFT", 1, Rect{64,0,64,64});
    m_pAnimator->Play(true);
}

void UC_Demo4_Ghost::Update(float delta)
{
    if (GameManager::Get()->GetDeltaVel() == 0.0f) return;

    UpdateMovement(delta);
    UpdateAnimation();
}

void UC_Demo4_Ghost::UpdateAnimation()
{
    if (m_currDir.x > 0)
        m_pAnimator->SetClip("RIGHT");
    else if(m_currDir.x < 0)
        m_pAnimator->SetClip("LEFT");
}

void UC_Demo4_Ghost::UpdateMovement(float delta)
{
    //check if future pos will get closer ot further from current pos

    Point nextPos = m_pTransform->GetPos() + (m_currDir * m_vel * delta);

    if (MathModule::Distance(m_currDest, nextPos) < MathModule::Distance(m_currDest, m_pTransform->GetPos()))
        m_pTransform->SetPos(nextPos);
    else
    {
        m_currDest = randomObj->GetRandomPoint(0, 500);
        m_currDir = MathModule::GetDirection(m_pTransform->GetPos(), m_currDest);
    }
}