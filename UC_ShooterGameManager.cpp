#include "UC_ShooterGameManager.h"
#include "UC_Includes.h"
#include "UC_Demo7_OrbEnemy.h"
#include "UC_Demo7_Player.h"
#include "UC_Demo7_Boss.h"
#include "UC_Bulltet.h"

UC_ShooterGameManager::UC_ShooterGameManager()
    : Component()
    , randomObj(Random::GetObject())
    , m_enemyOrbName("ENEMY_ORB_ENT")
    , m_enemyBossName("ENEMY_BOSS_ENT")
    , m_playerName("PLAYER_ENT")
    , m_bulletSrcRect(0,0,64,64)
    , m_playerTargetPoint(256, 512)
    , m_playerBullettype(1)
    , m_enemyBulletType(2)
    , m_playerBulletDir(1)
    , m_enemyBulletDir(-1)
    , m_orbs(0)
{

}

UC_ShooterGameManager::~UC_ShooterGameManager()
{}

void UC_ShooterGameManager::Init()
{
    UC_Demo7_OrbEnemy::m_pManager = this;
    UC_Demo7_Player::m_pManager = this;
    UC_Bullet::m_pManager = this;
    UC_Demo7_Boss::m_pManager = this;

    //Set up orb path points
    m_orbPathPoints.push_back(Point{ 100, 100 });
    m_orbPathPoints.push_back(Point{ 512 - 110, 100 });
    m_orbPathPoints.push_back(Point{ 512 - 140, 300 });
    m_orbPathPoints.push_back(Point{ 100, 256});
    m_orbPathPoints.push_back(Point{ 280 , 330});
    m_orbPathPoints.push_back(Point{ 90, 512 - 105});

    //Set up orb spawn points
    m_orbSpawnPoints.push_back(Point{ 256,-128 });
    m_orbSpawnPoints.push_back(Point{ -128,-128 });
    m_orbSpawnPoints.push_back(Point{ 512+128,-128 });
    m_orbSpawnPoints.push_back(Point{ -128, 256 });
    m_orbSpawnPoints.push_back(Point{ 512+128, 256 });
}

void UC_ShooterGameManager::Trigger(Event e, Entity* publisher)
{
    if (e == Event::SHOOTER_OBR_DESTROYED)
        m_orbs--;
}

Point UC_ShooterGameManager::GetRandomOrbPathPoint(UC_Demo7_OrbEnemy* unit)
{
    //Get random index
    size_t index = static_cast<size_t>(randomObj->GetRandomValue(static_cast<int>(0), static_cast<int>(m_orbPathPoints.size() - 1)));

    //holds the index of the units current point
    size_t currPosIndex = -1;

    //Get the unit's current point index (if it has one)
    for (size_t i = 0; i < m_orbPathPoints.size(); ++i)
    {
        if (m_orbPathPoints[i].m_pOwner == unit)
        {
            currPosIndex = i;
            break;
        }
    }

    //Look for a new random point that we can use for this unit
    //The random point must NOT be the same the unit already has
    //The random point must NOT be in use by any other unit (this means there's a limit for units)
    while (m_orbPathPoints[index].m_pOwner != nullptr ||
        m_orbPathPoints[index].m_pOwner == unit)
    {
        index = static_cast<size_t>(randomObj->GetRandomValue(static_cast<int>(0), static_cast<int>(m_orbPathPoints.size() - 1)));
    }

    //If the units had a point previously, make it vacan again
    if (currPosIndex != -1)
        m_orbPathPoints[currPosIndex].m_pOwner = nullptr;

    //make unit owner of new point
    m_orbPathPoints[index].m_pOwner = unit;

    //provide new point to unit
    return m_orbPathPoints[index].m_point;
}

void UC_ShooterGameManager::SpawnOrbs(int amnt)
{
    for (size_t i = 0; i < m_orbSpawnPoints.size(); ++i)
    {
        //if we at full capacity, just return.
        if (m_orbPathPoints.size() - 1 == m_orbs)
            return;

        if (amnt > 0)
        {
            auto orbEnt = m_entityManager->AddEntity(m_enemyOrbName);
            auto orbComp = orbEnt->AddComponent(new UC_Demo7_OrbEnemy(m_orbSpawnPoints[i]));

            ((IEventSender*)orbComp)->Sub(Event::SHOOTER_OBR_DESTROYED, this);

            --amnt;
            m_orbs++;
        }
        else
            break;
    }

    if (amnt > 0)
        SpawnOrbs(amnt);
}

void UC_ShooterGameManager::UpdateManager_OrbEnemyDestroyed(UC_Demo7_OrbEnemy* unit)
{
    for (size_t i = 0; i < m_orbPathPoints.size(); ++i)
    {
        if (m_orbPathPoints[i].m_pOwner == unit)
        {
            m_orbPathPoints[i].m_pOwner = nullptr;
            return;
        }
    }
}

Point UC_ShooterGameManager::GetPlayerTargetPoint()
{
    return m_playerTargetPoint;
}

Rect UC_ShooterGameManager::GetBulletSrcRect()
{
    return m_bulletSrcRect;
}

unsigned int UC_ShooterGameManager::GetPlayerBulletType()
{
    return m_playerBullettype;
}

unsigned int UC_ShooterGameManager::GetEnemyBulletType()
{
    return m_enemyBulletType;
}

int UC_ShooterGameManager::GetPlayerBulletDir()
{
    return m_playerBulletDir;
}

int UC_ShooterGameManager::GetEnemyBulletDir()
{
    return m_enemyBulletDir;
}

const char* UC_ShooterGameManager::GetEnemyOrbName()
{
    return m_enemyOrbName;
}

const char* UC_ShooterGameManager::GetEnemyBossName()
{
    return m_enemyBossName;
}

const char* UC_ShooterGameManager::GetPlayerName()
{
    return m_playerName;
}