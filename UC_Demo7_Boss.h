#pragma once
#include "Component.h"
#include "IShooterHittable.h"
#include "EventSystem.h"
#include "CoreStructs.h"
#include <string>
#include <vector>

class EC_Collider;
class EC_Transform;
class UC_ShooterGameManager;
class UC_Shooter;
class UC_Dust;
class UC_Demo7_ShieldPiece;
class UC_ShieldTrigger;

class UC_Demo7_Boss : public Component, public IShooterHittable, public IEventListener
{
    friend class UC_ShooterGameManager;

private:
    enum class BossState {k_Shoot, k_Missile, k_Shield, k_Dead};
    //Shooting: shots 1 bullet every x time
    //Missle launching: charges its weapon then launches 3 missles to the air that come back
    //and hit the player (launches them in a fast succession)
    //Shielding: charges as shield and stays put while orb enemies spawn non-stop until
    //you destroy the shield. Once the shield its destroyed, state 2 happens.

    //event: every time the health drops 1/3, orbs spawn

public:
    UC_Demo7_Boss();
    virtual ~UC_Demo7_Boss() override;
    virtual void Trigger(Event e, Entity* publisher) override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void OnHit(float dmg) override;
    void SetState(BossState newState);
    void UpdateState(float delta);
    void InitShield();//prepares the shield so it can be used later.
    void EnableShieldCol(bool value);
    void SummonShield();
    void DamageShield();
    void SpawnOrbs(float delta);
    void UpdateMovement(float delta);

private:
    static UC_ShooterGameManager* m_pManager;
    std::vector<Point> m_shieldPiecesPos;
    std::vector<UC_Demo7_ShieldPiece*> m_shieldPieces;
    std::vector<UC_ShieldTrigger*> m_shieldTriggers;
    std::vector<Point> m_path;
    UC_Shooter* m_pShooter;
    EC_Transform* m_pTransform;
    EC_Collider* m_pCol;
    BossState m_currState;
    float m_startHP;
    float m_currHP;
    Point m_bossPos;
    Point m_bulletSpawnPivot;
    Point m_shieldPos;
    float m_shieldPX;
    float m_shieldPY;
    unsigned int m_shieldPiecesCounter;
    bool m_shieldCompleted;
    unsigned int m_stateCounter;
    bool m_spawnOrbsPeriodically;
    float m_orbSpawnRate;
    float m_orbSpawnCounter;
    unsigned int m_currPathStart;
    float m_vel;

    //SHOOTING state
    std::string m_stateShoot_bulletImg;
    float m_stateShoot_fireRate;
    float m_stateShoot_fireRateCounter;
    float m_stateShoot_bulletDmg;
    float m_stateShoot_bulletVel;
    float m_stateShoot_minBulletScale;
    float m_stateShoot_maxBulletScale;

    //MISSLE state
    std::string m_stateMissle_missleImg;
    Point m_stateMissle_missleUpTargetPos;
    float m_stateMissle_chargeTime;
    float m_stateMissle_chargeTimeCounter;
    float m_stateMissle_inBetCharge;
    float m_stateMissle_inBetChargeCounter;
    unsigned int m_stateMissle_misslesToSpawn;
    unsigned int m_stateMissle_misslesToSpawnCounter;
    float m_stateMissle_missleDmg;
    float m_stateMissle_missleVel;
    float m_stateMissle_upMissleScale;
    float m_stateMissle_downMissleScale;
};