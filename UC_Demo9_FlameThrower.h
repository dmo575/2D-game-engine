#pragma once
#include "Component.h"
#include "IShooterHittable.h"


class Entity;
class EC_Transform;
class EC_Sprite;
class EC_Shape;
class EC_ParticleSpawner;

class UC_Demo9_Flamethrower : public Component
{
public:
    UC_Demo9_Flamethrower();
    virtual ~UC_Demo9_Flamethrower() override;

    Point GetCurrentFlamePos();
    bool ThrowingFlames();

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

    void UpdateAim(float delta);
    void CreateBarrelPointer();
    void CreateBarrelShape();
    void UpdateBarrelEndPos();
    void UpdateBarrelShape();
    void UpdateParticleSpawner();
    void UpdateInput();

private:
    Entity* m_aimEnt;
    EC_Transform* m_aimTransform;
    EC_Transform* m_barrelTransform;
    EC_Shape* m_barrelShape;
    EC_Sprite* m_aimSprite;
    EC_ParticleSpawner* m_pParticleSpawner;
    EC_Transform* m_pSpawnerTransform;


    Point m_barrelOriginPos;
    Point m_barrelEndPos;
    Point m_preRecoilBarrelEndPos;
    Point m_preRecoilBarrelOriginPos;
    float m_barrelLength;
    float m_barrelOriginShapeLength;
    float m_barrelEndShapeLength;
    float m_flameConeMin;
    float m_flameConeMax;
};