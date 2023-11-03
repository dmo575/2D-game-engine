#include "UC_Demo7_Boss.h"
#include "UC_Includes.h"
#include "UC_Shooter.h"
#include "UC_ShooterGameManager.h"
#include "UC_Demo7_ShieldPiece.h"
#include "UC_ShieldTrigger.h"
#include <iostream>

UC_ShooterGameManager* UC_Demo7_Boss::m_pManager = nullptr;

UC_Demo7_Boss::UC_Demo7_Boss()
    : m_pShooter(nullptr)
    , m_pCol(nullptr)
    , m_pTransform(nullptr)
    , m_currState(BossState::k_Shoot)
    , m_startHP(40)
    , m_currHP(m_startHP)
    , m_bossPos(256, 168)
    , m_bulletSpawnPivot(230, 240)
    , m_shieldPos(256, 200)
    , m_shieldPX((64/2)/2)
    , m_shieldPY((102 - (96/3))/2)
    , m_shieldPiecesCounter(0)
    , m_shieldCompleted(false)
    , m_stateCounter(0)
    , m_spawnOrbsPeriodically(false)
    , m_currPathStart(0)
    , m_vel(100.0f)
    , m_stateShoot_bulletImg("Shooter/SP_DummyBox.png")
    , m_stateShoot_fireRate(1.0f)
    , m_stateShoot_fireRateCounter(0.0f)
    , m_stateShoot_bulletDmg(1.0f)
    , m_stateShoot_bulletVel(20.0f)
    , m_stateShoot_minBulletScale(1.0f)
    , m_stateShoot_maxBulletScale(150.0f)
    , m_stateMissle_missleImg("Shooter/SP_DummyBox.png")
    , m_stateMissle_missleUpTargetPos(m_bulletSpawnPivot + Point{0, -200})
    , m_stateMissle_chargeTime(3.0f)
    , m_stateMissle_chargeTimeCounter(0.0f)
    , m_stateMissle_inBetCharge(0.25f)
    , m_stateMissle_inBetChargeCounter(0.0f)
    , m_stateMissle_misslesToSpawn(3)
    , m_stateMissle_misslesToSpawnCounter(0)
    , m_stateMissle_missleDmg(2)
    , m_stateMissle_missleVel(10.0f)
    , m_stateMissle_upMissleScale(50.0f)
    , m_stateMissle_downMissleScale(260.0f)
    , m_orbSpawnCounter(0.0f)
    , m_orbSpawnRate(3.0f)
{}

UC_Demo7_Boss::~UC_Demo7_Boss()
{}

void UC_Demo7_Boss::Init()
{
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    auto sprite = m_pParent->AddComponent(new EC_Sprite);
    m_pCol = m_pParent->AddComponent(new EC_Collider);

    m_pShooter = m_pParent->AddComponent(new UC_Shooter);
    m_pShooter->AddTarget(m_pManager->GetPlayerName());
    m_pTransform->SetPos(m_bossPos);
    m_pTransform->SetScale(0.25f, 0.25f);

    Rect srcImg{ 0,0,1000,1000 };
    sprite->SetSourceImg("Shooter/SS_ENEMY_BOSS3.png", srcImg);
    sprite->SetPivotToCenter();

    m_pCol->AttachToSprite(false);
    Rect colRect{-60, -45, 125, 130};
    m_pCol->SetColRect(colRect);

    
    //test path
    m_path.push_back({ 50,20 });
    m_path.push_back({ 256,50 });
    m_path.push_back({ 480,480 });
    m_path.push_back({ 200,320 });


    InitShield();
    SetState(BossState::k_Shoot);
}

void UC_Demo7_Boss::Update(float delta)
{
    if (GameManager::Get()->GetDeltaVel() == 0.0f) return;

    UpdateState(delta);
    SpawnOrbs(delta);
}

void UC_Demo7_Boss::UpdateMovement(float delta)
{
    size_t pathEnd = m_currPathStart < (m_path.size() - 1) ? m_currPathStart + 1 : 0;
    Point dir = MathModule::GetDirection(m_path[m_currPathStart], m_path[pathEnd]);

    m_pTransform->AddPos(dir * m_vel * delta);


    if (MathModule::Distance(m_path[m_currPathStart], m_path[pathEnd]) <
        MathModule::Distance(m_path[m_currPathStart], m_pTransform->GetPos()))
    {
        m_pTransform->SetPos(m_path[pathEnd]);
        m_currPathStart++;
        m_currPathStart = m_currPathStart > m_path.size() - 1 ? static_cast<size_t>(0) : m_currPathStart;
    }
}

void UC_Demo7_Boss::OnHit(float dmg)
{
    m_currHP--;

    if (m_currHP < (m_startHP / 6) * 5 && m_stateCounter == 0)
    {
        m_pManager->SpawnOrbs(7);
        m_stateCounter++;
    }
    else if (m_currHP < (m_startHP / 3) * 2 && m_stateCounter == 1)
    {
        m_stateCounter++;
        SetState(BossState::k_Missile);
        m_pManager->SpawnOrbs(7);
    }
    else if (m_currHP < (m_startHP / 6) * 3 && m_stateCounter == 2)
    {
        m_pManager->SpawnOrbs(7);
        m_stateCounter++;
    }
    else if (m_currHP < (m_startHP / 3) && m_stateCounter == 3)
    {
        m_stateCounter++;
        SetState(BossState::k_Shield);
        m_pManager->SpawnOrbs(7);
        m_spawnOrbsPeriodically = true;
    }

    if (m_currHP <= 0 && m_currState != BossState::k_Dead)
    {
        SetState(BossState::k_Dead);
        GameManager::Get()->ShowCursor(true);
        LevelManager::Get()->QueueLoadLevel("demo_9_lvl1");
    }
}

void UC_Demo7_Boss::SpawnOrbs(float delta)
{
    if (!m_spawnOrbsPeriodically) return;
    m_orbSpawnCounter += delta;

    if (m_orbSpawnCounter >= m_orbSpawnRate)
    {
        m_orbSpawnCounter -= m_orbSpawnRate;
        m_pManager->SpawnOrbs(2);
    }
}

void UC_Demo7_Boss::SetState(BossState newState)
{

    //De-init curr state
    switch (m_currState)
    {
    case UC_Demo7_Boss::BossState::k_Shield:
    {
        m_pCol->SetActive(true);
        EnableShieldCol(false);

        for (size_t i = 0; i < m_shieldPieces.size(); ++i)
            m_shieldPieces[i]->GetPieceOut();
        break;
    }
    }

    m_currState = newState;

    //Init curr state
    switch (m_currState)
    {
    case UC_Demo7_Boss::BossState::k_Shoot:
    {
        m_pShooter->SetProjectile(m_pManager->GetEnemyBulletType(), m_stateShoot_bulletImg.c_str(), m_pManager->GetBulletSrcRect(), m_stateShoot_bulletVel, m_stateShoot_bulletDmg, -1, 1.0f, 150.0f);
        m_stateShoot_fireRateCounter = 0.0f;
        break;
    }
    case UC_Demo7_Boss::BossState::k_Missile:
    {
        //This bullet is set up to be missle UP. we should have a missle class and use that instead of shooter. or have a missle inside shooter class
        m_pShooter->SetProjectile(m_pManager->GetEnemyBulletType(), m_stateMissle_missleImg.c_str(), m_pManager->GetBulletSrcRect(), m_stateMissle_missleVel, m_stateMissle_missleDmg, -1, m_stateMissle_upMissleScale, m_stateMissle_downMissleScale);
        m_stateMissle_chargeTimeCounter = 0.0f;
        m_stateMissle_inBetChargeCounter = 0.0f;
        break;
    }
    case UC_Demo7_Boss::BossState::k_Shield:
    {
        SummonShield();
        break;
    }
    }
}

void UC_Demo7_Boss::UpdateState(float delta)
{
    switch (m_currState)
    {
    case UC_Demo7_Boss::BossState::k_Shoot:
    {
        m_stateShoot_fireRateCounter += delta;

        if (m_stateShoot_fireRateCounter >= m_stateShoot_fireRate)
        {
            Point playerPos{256, 512};
            m_pShooter->SpawnBullet(m_bulletSpawnPivot, playerPos);
            m_stateShoot_fireRateCounter -= m_stateShoot_fireRate;
        }
        break;
    }
    case UC_Demo7_Boss::BossState::k_Missile:
    {
        m_stateMissle_chargeTimeCounter += delta;

        //if charge complete, start inner charge (first time it starts charged)
        if (m_stateMissle_chargeTimeCounter >= m_stateMissle_chargeTime)
        {
            m_stateMissle_inBetChargeCounter += delta;

            //if in-bet charge complete, spawn missle
            if (m_stateMissle_inBetChargeCounter >= m_stateMissle_inBetCharge)
            {
                //spawn missle
                //reset in bet counter
                //set misslesSpawned to +1
                m_pShooter->SpawnMissleUP(m_bulletSpawnPivot, m_pManager->GetPlayerTargetPoint(), 2.0f);
                m_stateMissle_misslesToSpawnCounter++;
                m_stateMissle_inBetChargeCounter -= m_stateMissle_inBetCharge;
                if (m_stateMissle_misslesToSpawnCounter >= m_stateMissle_misslesToSpawn)
                {
                    m_stateMissle_misslesToSpawnCounter = 0;
                    m_stateMissle_chargeTimeCounter = 0.0f;
                    m_stateMissle_inBetChargeCounter = m_stateMissle_inBetCharge;
                }
            }
        }
        break;
    }
    }
}

void UC_Demo7_Boss::Trigger(Event e, Entity* publisher)
{
    switch (e)
    {
    case Event::SHOOTER_SHIELD_DAMAGED:
    {
        if (m_shieldCompleted)
            DamageShield();
        break;
    }
    case Event::SHOOTER_SHIELDPIECE_ARRIVED:
    {
        m_shieldPiecesCounter++;
        m_pCol->SetActive(false);

        if (m_shieldPiecesCounter == 19)
        {
            m_shieldCompleted = true;
            EnableShieldCol(true);
        }
        break;
    }
    }
}

void UC_Demo7_Boss::InitShield()
{
    auto shieldTrigger1 = m_entityManager->AddEntity("SHIELD_TRIGGER_ENT");
    auto shieldTriggerComp1 = shieldTrigger1->AddComponent(new UC_ShieldTrigger(m_shieldPos, Rect{-64, -56, 128, 112}));
    ((IEventSender*)shieldTriggerComp1)->Sub(Event::SHOOTER_SHIELD_DAMAGED, this);
    auto shieldTrigger2 = m_entityManager->AddEntity("SHIELD_TRIGGER_ENT");
    auto shieldTriggerComp2 = shieldTrigger2->AddComponent(new UC_ShieldTrigger(m_shieldPos, Rect{ -64+16, +56, 96, 32 }));
    ((IEventSender*)shieldTriggerComp2)->Sub(Event::SHOOTER_SHIELD_DAMAGED, this);
    auto shieldTrigger3 = m_entityManager->AddEntity("SHIELD_TRIGGER_ENT");
    auto shieldTriggerComp3 = shieldTrigger3->AddComponent(new UC_ShieldTrigger(m_shieldPos, Rect{ -64 + 16, -56 - 32, 96, 32 }));
    ((IEventSender*)shieldTriggerComp3)->Sub(Event::SHOOTER_SHIELD_DAMAGED, this);
    auto shieldTrigger4 = m_entityManager->AddEntity("SHIELD_TRIGGER_ENT");
    auto shieldTriggerComp4 = shieldTrigger4->AddComponent(new UC_ShieldTrigger(m_shieldPos, Rect{ -64 - 16, -16, 16, 32 }));
    ((IEventSender*)shieldTriggerComp4)->Sub(Event::SHOOTER_SHIELD_DAMAGED, this);
    auto shieldTrigger5 = m_entityManager->AddEntity("SHIELD_TRIGGER_ENT");
    auto shieldTriggerComp5 = shieldTrigger5->AddComponent(new UC_ShieldTrigger(m_shieldPos, Rect{ +64, -16, 16, 32 }));
    ((IEventSender*)shieldTriggerComp5)->Sub(Event::SHOOTER_SHIELD_DAMAGED, this);
    
    m_shieldTriggers.push_back(shieldTriggerComp1);
    m_shieldTriggers.push_back(shieldTriggerComp2);
    m_shieldTriggers.push_back(shieldTriggerComp3);
    m_shieldTriggers.push_back(shieldTriggerComp4);
    m_shieldTriggers.push_back(shieldTriggerComp5);

    EnableShieldCol(false);

    m_shieldPiecesPos.reserve(19);
    m_shieldPiecesPos.push_back(m_shieldPos);
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[0]  + Point {   m_shieldPX, -m_shieldPY});
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[1]  + Point { +m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[2]  + Point { -m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[3]  + Point { -(m_shieldPX + 32/2), 0.0f });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[4]  + Point { -m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[5]  + Point { +m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[6]  + Point { +m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[7]  + Point { +(m_shieldPX + 32/2), 0.0f });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[8]  + Point { +m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[9]  + Point { +m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[10] + Point{ -m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[11] + Point{ -m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[12] + Point{ -(m_shieldPX + 32/2), 0.0f});
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[13] + Point{ -(m_shieldPX + 32/2), 0.0f });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[14] + Point{ -m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[15] + Point{ -m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[16] + Point{ +m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[17] + Point{ +m_shieldPX, -m_shieldPY });

    Point temp = m_shieldPos + Point{ 1,1 };

    //create the shield ents
    for (size_t i = 0; i < 19; ++i)
    {
        auto shieldEnt = m_entityManager->AddEntity("SHIELD_PIECE_ENT");

        Point spawnPos = MathModule::GetDirection(temp, m_shieldPiecesPos[i]);

        auto shieldPiece = shieldEnt->AddComponent(new UC_Demo7_ShieldPiece(spawnPos * 700.0f));
        m_shieldPieces.push_back(shieldPiece);

        ((IEventSender*)m_shieldPieces[i])->Sub(Event::SHOOTER_SHIELDPIECE_ARRIVED, this);
    }
}

void UC_Demo7_Boss::EnableShieldCol(bool value)
{
    for (size_t i = 0; i < m_shieldTriggers.size(); ++i)
        m_shieldTriggers[i]->Enable(value);
}

void UC_Demo7_Boss::SummonShield()
{
    //to every piece, tell it to gotowards their pos.
    //use event system to let robot know when they reac h their pos
    for (size_t i = 0; i < m_shieldPieces.size(); ++i)
        m_shieldPieces[i]->MoveShieldTo(m_shieldPiecesPos[i]);
}

void UC_Demo7_Boss::DamageShield()
{
    for (size_t i = 19; i != 0; --i)
    {
        if (m_shieldPieces[i - static_cast<size_t>(1)]->GetState() != UC_Demo7_ShieldPiece::ShieldPieceState::k_Destroyed)
        {
            m_shieldPieces[i - static_cast<size_t>(1)]->DamagePiece();
            break;
        }
    }

    if (m_shieldPieces[0]->GetState() == UC_Demo7_ShieldPiece::ShieldPieceState::k_Destroyed)
        SetState(BossState::k_Missile);
}