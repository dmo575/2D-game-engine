#pragma once
#include "Component.h"
#include "IShooterHittable.h"

class Entity;
class EC_Transform;
class EC_Sprite;
class EC_Shape;
class UC_Shooter;
class UC_ShooterGameManager;

class UC_Demo7_Player : public Component, public IShooterHittable
{
    friend class UC_ShooterGameManager;
public:
    UC_Demo7_Player();
    virtual ~UC_Demo7_Player() override;
    virtual void OnHit(float dmg) override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

    void UpdateAim(float delta);
    void UpdateShoot(float delta);
    void CanAim(bool value);
    void CanShoot(bool value);
    void CanParry(bool value);
    void CreateBarrelPointer();
    void CreateBarrelShape();
    void UpdateBarrelEndPos();
    void UpdateBarrelShape();
    void UpdateRecoil(float delta);
private:
    static UC_ShooterGameManager* m_pManager;
    Entity* m_aimEnt;
    EC_Transform* m_aimTransform;
    EC_Transform* m_barrelTransform;
    EC_Shape* m_barrelShape;
    EC_Sprite* m_aimSprite;
    UC_Shooter* m_shooter;
    unsigned int m_hp;
    float m_bulletMinScalePer;
    float m_bulletMaxScalePer;
    bool m_canAim;
    bool m_canShoot;
    bool m_canParry;
    unsigned int m_recoilState;//0 no, 1 up, 2 down
    float m_recoilDistance;
    float m_recoilVelBack;
    float m_recoilVelForward;

    Point m_barrelOriginPos;
    Point m_barrelEndPos;
    Point m_preRecoilBarrelEndPos;
    Point m_preRecoilBarrelOriginPos;
    float m_barrelLength;
    float m_barrelOriginShapeLength;
    float m_barrelEndShapeLength;
};