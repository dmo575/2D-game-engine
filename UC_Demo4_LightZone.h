#pragma once
#include "Component.h"//        Superclass

class EC_Transform;
class EC_Sprite;
class EC_Collider;
class Entity;

class UC_Demo4_LightZone : public Component
{
public:
    UC_Demo4_LightZone(Entity* toFollow);
    virtual ~UC_Demo4_LightZone() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Transform* m_pTransform;
    Entity* m_pToFollow;
    EC_Transform* m_toFollorTransform;
};