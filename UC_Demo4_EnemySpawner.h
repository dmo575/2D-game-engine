#pragma once
#include "Component.h"//        Superclass

class EC_Transform;
class EC_Sprite;
class EC_Collider;
class Entity;
class EC_Sound;

class UC_Demo4_EnemySpawner : public Component
{
public:
    UC_Demo4_EnemySpawner(float spawnRatio);
    virtual ~UC_Demo4_EnemySpawner() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Transform* m_pTransform;
    EC_Sound* m_pSound;
    float m_spawnRatio;
    float m_spawnCounter;
};