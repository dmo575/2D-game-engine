#pragma once
#include "Component.h"
#include "IShooterHittable.h"
#include "EventSystem.h"
#include <memory>

class EC_Transform;
class EC_Sprite;
class EC_Collider;
class EC_Animator;
class UC_ShooterGameManager;
class UC_Shooter;
class Entity;

class UC_Demo7_OrbEnemy : public Component, public IShooterHittable, public IEventSender
{
    friend UC_ShooterGameManager;
private:
    enum class OrbStates {k_Idle = 0, k_Shoot, k_Move, k_No_State};
    //IDLE: Stays in position for X seconds
    //SHOOT: Spawns a bullet aimed at player
    //MOVE: Moves towards a pre-defined point

    //How Orb behaves:
    //IDLE > SHOOT > IDLE > MOVE

public:
    UC_Demo7_OrbEnemy(Point spawnPoint);
    virtual ~UC_Demo7_OrbEnemy() override;
    virtual void OnHit(float dmg) override;
    virtual void Sub(Event e, IEventListener* listener) override;
    virtual void Unsub(Event e, IEventListener* listener) override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    void SetState(OrbStates newState);
    void SpawnCannonFireSparks();
    void DestroyOrb();

    //STATE IDLE
    void State_Idle_Init();
    void State_Idle_Update(float delta);

    //STATE MOVE
    void State_Move_Init();
    void State_Move_Update(float delta);

    //STATE SHOOT
    void State_Shoot_Init();

    //State independant behaivours
    void UpdateIdleFloatingBehaivour(float delta);


private:
    static UC_ShooterGameManager* m_pManager;
    Point m_spawnPoint;
    Point m_bulletSpawnOffset;
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    EC_Collider* m_pCol;
    EC_Animator* m_pAnimator;
    std::unique_ptr<OEventSender> m_pEventSender;
    OrbStates m_currState;
    UC_Shooter* m_pShooter;
    const char* m_spriteSrcImg;
    const char* m_bulletSpriteSrcImg;
    float m_unitVel;
    float m_bulletVel;
    float m_bulletDmg;
    float m_bulletMinScalePer;
    float m_bulletMaxScalePer;

    //IDLE STATE VARIABLES
    bool m_stateIdle_goShoot;
    float m_stateIdle_currWaitTime;
    float m_stateIdle_maxWaitTime;

    //MOVE STATE VARIABLES
    Point m_stateMove_start;
    Point m_stateMove_destination;
    Point m_stateMove_direction;
    float m_stateMove_travelTotalDistance;
};