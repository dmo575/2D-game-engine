#pragma once
#include "Component.h"
#include <vector>
#include "CoreStructs.h"
#include "EventSystem.h"
#include "Random.h"

class UC_Demo7_OrbEnemy;

class UC_ShooterGameManager : public Component, public IEventListener
{
private:
    struct PointData
    {
    public:
        PointData(Point p)
            : m_point(p)
            , m_pOwner(nullptr)
        {};

    public:
        Point m_point;
        UC_Demo7_OrbEnemy* m_pOwner;
    };

public:
    UC_ShooterGameManager();
    virtual ~UC_ShooterGameManager() override;
    virtual void Trigger(Event e, Entity* publisher) override;

    void UpdateManager_OrbEnemyDestroyed(UC_Demo7_OrbEnemy* unit);

    void SpawnOrbs(int amnt);
    Point GetRandomOrbPathPoint(UC_Demo7_OrbEnemy* unit);
    Rect GetBulletSrcRect();
    Point GetPlayerTargetPoint();
    unsigned int GetPlayerBulletType();
    unsigned int GetEnemyBulletType();
    int GetEnemyBulletDir();
    int GetPlayerBulletDir();
    const char* GetEnemyOrbName();
    const char* GetEnemyBossName();
    const char* GetPlayerName();


private:
    virtual void Init() override;

private:
    std::vector<PointData> m_orbPathPoints;
    std::vector<Point> m_orbSpawnPoints;
    std::unique_ptr<Random::RandomObject> randomObj;
    const char* m_enemyOrbName;
    const char* m_enemyBossName;
    const char* m_playerName;

    Rect m_bulletSrcRect;
    Point m_playerTargetPoint;
    unsigned int m_playerBullettype;
    unsigned int m_enemyBulletType;
    int m_playerBulletDir;
    int m_enemyBulletDir;
    unsigned int m_orbs;
};
