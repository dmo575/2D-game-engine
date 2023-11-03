#pragma once
#include "Component.h"//        Superclass

class Entity;
class EC_Transform;
class EC_Sprite;

class UC_StaticTracker : public Component
{
public:
    UC_StaticTracker(EC_Transform* toTrack);
    virtual ~UC_StaticTracker() override;

    void SetTarget(EC_Transform* newTarget);
    void SetRange(float newRange);

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

    float GetDif(float a, float b);

private:
    EC_Transform* m_pTarget;
    EC_Sprite* m_pSprite;
    EC_Transform* m_pTransform;
    float m_range;
};