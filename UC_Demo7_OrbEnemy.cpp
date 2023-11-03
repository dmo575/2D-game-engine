#include "UC_Demo7_OrbEnemy.h"
#include "UC_Includes.h"
#include "UC_Shooter.h"
#include "UC_ShooterGameManager.h"
#include "UC_ExplodingEffect.h"
#include "UC_Dust.h"
#include <iostream>

//UC_Demo7_EnemyManager sets this up itself.
UC_ShooterGameManager* UC_Demo7_OrbEnemy::m_pManager = nullptr;

UC_Demo7_OrbEnemy::UC_Demo7_OrbEnemy(Point spawnPoint)
    : Component()
    , m_spawnPoint(spawnPoint)
    , m_bulletSpawnOffset(-15, 15)
    , m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_pCol(nullptr)
    , m_pAnimator(nullptr)
    , m_pShooter(nullptr)
    , m_currState(OrbStates::k_No_State)
    , m_spriteSrcImg("Shooter/SS_ENEMY_ORB.png")
    , m_bulletSpriteSrcImg("Shooter/C_Yellow.png")
    , m_unitVel(550.0f)
    , m_bulletVel(10.0f)
    , m_bulletDmg(1.0f)
    , m_stateIdle_goShoot(false)
    , m_stateIdle_currWaitTime(0.0f)
    , m_stateIdle_maxWaitTime(1.5f)
    , m_stateMove_start(0,0)
    , m_stateMove_destination(0,0)
    , m_stateMove_direction(0,0)
    , m_stateMove_travelTotalDistance(0.0f)
    , m_bulletMinScalePer(1.0f)
    , m_bulletMaxScalePer(40.0f)
{}

UC_Demo7_OrbEnemy::~UC_Demo7_OrbEnemy()
{
    m_pManager->UpdateManager_OrbEnemyDestroyed(this);
}

void UC_Demo7_OrbEnemy::Init()
{
    //Check for dependency
    if (!m_pManager)
        Logger::Log(Logger::LOG_LEVEL::k_Error, "UC_ShooterManager is not accessible. You can't instantiate UC_Demo7_OrbEnemy before UC_ShooterManager.");

    m_pEventSender = std::make_unique<OEventSender>(m_pParent);

    //Get needed data
    Rect bulletSrcRect = m_pManager->GetBulletSrcRect();
    bulletSrcRect = {0,0,128,128};
    unsigned int bulletType = m_pManager->GetEnemyBulletType();
    int bulletDir = m_pManager->GetEnemyBulletDir();
    Rect orbSrcRect{0,0,512,512};

    //Add components
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite);
    m_pShooter = m_pParent->AddComponent(new UC_Shooter);
    m_pCol = m_pParent->AddComponent(new EC_Collider);
    m_pAnimator = m_pParent->AddComponent(new EC_Animator);

    //Set up Shooter comp
    m_pShooter->SetProjectile(bulletType, m_bulletSpriteSrcImg, bulletSrcRect, m_bulletVel, m_bulletDmg, bulletDir, m_bulletMinScalePer, m_bulletMaxScalePer);
    m_pShooter->AddTarget(m_pManager->GetPlayerName());

    //Set up transform
    m_pTransform->SetPos(m_spawnPoint);
    m_pTransform->SetScale(0.5f, 0.5f);

    //Set up sprite
    m_pSprite->SetSourceImg(m_spriteSrcImg, orbSrcRect);
    m_pSprite->SetPivot(Point{-128, -128});

    //Set up collider
    m_pCol->AttachToSprite(false);
    Rect colRect{ -25,-30,50,50 };
    m_pCol->SetColRect(colRect);
    m_pCol->SetTrigger(true);

    //Set up animator
    m_pAnimator->SetSpeed(0.025f);
    m_pAnimator->AddClip("IDLE", 1, Rect{0,0,256,256});
    m_pAnimator->AddClip("SHOOT", 8, Rect{ 0,256,256,256});
    m_pAnimator->Play(true);

    SetState(OrbStates::k_Move);
}

void UC_Demo7_OrbEnemy::Sub(Event e, IEventListener* listener)
{
    m_pEventSender->Sub(e, listener);
}

void UC_Demo7_OrbEnemy::Unsub(Event e, IEventListener* listener)
{
    m_pEventSender->Unsub(e, listener);
}

void UC_Demo7_OrbEnemy::Update(float delta)
{
    if (GameManager::Get()->GetDeltaVel() == 0.0f) return;

    //Update current state (if needed)
    switch (m_currState)
    {
    case OrbStates::k_Idle:
    {
        State_Idle_Update(delta);
        break;
    }

    case OrbStates::k_Move:
    {
        State_Move_Update(delta);
        break;
    }
    }
}

void UC_Demo7_OrbEnemy::SetState(OrbStates newState)
{
    if (m_currState == newState)
        Logger::Log(Logger::LOG_LEVEL::k_Warning, "UC_Demo7_OrbEnemy: Trying to set the current state as the new state");


    //De-init current state (if needed)
    //N/A

    m_currState = newState;

    //Init new state (if needed)
    switch (m_currState)
    {
    case OrbStates::k_Idle:
    {
        State_Idle_Init();
        m_pAnimator->SetClip("IDLE");
        break;
    }

    case OrbStates::k_Move:
    {
        State_Move_Init();
        m_pAnimator->SetClip("IDLE");
        break;
    }

    case OrbStates::k_Shoot:
    {
        State_Shoot_Init();
        m_pAnimator->SetClip("SHOOT", false, false);
        break;
    }
    }
}

void UC_Demo7_OrbEnemy::State_Idle_Init()
{
    //Update the boolean, this determines wether you cann MOVE or SHOOT after the state ends
    m_stateIdle_goShoot = !m_stateIdle_goShoot;

    //Reset/Set up state variables
    m_stateIdle_currWaitTime = 0.0f;
}

void UC_Demo7_OrbEnemy::State_Idle_Update(float delta)
{
    //update time
    m_stateIdle_currWaitTime += delta;

    //If we completed the time we had to stay in idle, calculate next state to go to and call it
    if (m_stateIdle_currWaitTime >= m_stateIdle_maxWaitTime)
    {
        OrbStates nextState = m_stateIdle_goShoot ? OrbStates::k_Shoot : OrbStates::k_Move;
        SetState(nextState);
    }
}


void UC_Demo7_OrbEnemy::State_Move_Init()
{
    //Get new destination (manager will make sure that its not the same and that its not being used already)
    Point nextPos = m_pManager->GetRandomOrbPathPoint(this);
    m_stateMove_start = m_pTransform->GetPos();


    m_stateMove_destination = nextPos;
    m_stateMove_direction = MathModule::GetDirection(m_stateMove_start, m_stateMove_destination);
    m_stateMove_travelTotalDistance = MathModule::Distance(m_stateMove_start, m_stateMove_destination);
}


//NOTE: I just realized is when you have a fixed place as a destination, its much easier to
//check wether you have traveled the distance or more rather than "are you close enough to dest"
void UC_Demo7_OrbEnemy::State_Move_Update(float delta)
{
    //Move the orb toawrds its destination
    m_pTransform->AddPos(m_stateMove_direction * m_unitVel * delta);

    //get distance from start to current pos
    float distanceFromStart = MathModule::Distance(m_pTransform->GetPos(), m_stateMove_start);

    //if distance from start to current pos is equal or more than dist from start to end pos, we arrived.
    if (distanceFromStart >= m_stateMove_travelTotalDistance)
        SetState(OrbStates::k_Idle);
}

void UC_Demo7_OrbEnemy::State_Shoot_Init()
{
    Point playerTargetPoint = m_pManager->GetPlayerTargetPoint();

    m_pShooter->SpawnBullet(m_pTransform->GetPos() + m_bulletSpawnOffset, playerTargetPoint);
    SpawnCannonFireSparks();
    SetState(OrbStates::k_Idle);
}

void ::UC_Demo7_OrbEnemy::UpdateIdleFloatingBehaivour(float delta)
{
    m_pTransform->AddPos(0.0f, 0.0f);
}

void UC_Demo7_OrbEnemy::OnHit(float dmg)
{
    DestroyOrb();
}

void UC_Demo7_OrbEnemy::DestroyOrb()
{
    Rect srcRect{0, 0, 365, 365};
    float anmSpeed = 0.05f;
    auto explosion = m_entityManager->AddEntity("EXPLOSION_ORB");
    std::string spriteName = "Shooter/SS_ENEMY_ORB_EXPLODE.png";
    explosion->AddComponent(new UC_ExplodingEffect(spriteName.c_str(), srcRect, 4, anmSpeed, m_pTransform->GetPos(), m_pTransform->GetScale().x));
    m_pEventSender->TriggerEvent(Event::SHOOTER_OBR_DESTROYED);
    m_pParent->Destroy();
}

void UC_Demo7_OrbEnemy::SpawnCannonFireSparks()
{
    //spawn various small dust ents going in all directions at fast speed
    Point directions[8];
    directions[0] = MathModule::GetDirection(Point{ 0,0 }, Point{ 0,-1 });
    directions[1] = MathModule::GetDirection(Point{ 0,0 }, Point{ 0,1 });
    directions[2] = MathModule::GetDirection(Point{ 0,0 }, Point{ 1,0 });
    directions[3] = MathModule::GetDirection(Point{ 0,0 }, Point{ -1,0 });
    directions[4] = MathModule::GetDirection(Point{ 0,0 }, Point{ 1,-1 });
    directions[5] = MathModule::GetDirection(Point{ 0,0 }, Point{ -1,-1 });
    directions[6] = MathModule::GetDirection(Point{ 0,0 }, Point{ 1,1 });
    directions[7] = MathModule::GetDirection(Point{ 0,0 }, Point{ 1,-1 });

    for (size_t i = 0; i < 8; ++i)
    {
        std::string spriteName = "Shooter/C_Yellow.png";
        Rect srcRect{ 0, 0, 10, 10 };
        float vel = 300.0f;
        float scaleMult = 1.1f;
        float lifeSpam = 0.08f;
        unsigned int layer = m_pSprite->GetLayer() + 1;
        auto dustEnt = m_entityManager->AddEntity("DUST_ENT");
        float scale = 1.0f;
        dustEnt->AddComponent(new UC_Dust(scale, 1, spriteName.c_str(), srcRect, m_pTransform->GetPos() + Point{-15, 15}, directions[i] * -1, vel, scaleMult, lifeSpam, layer));
    }
}