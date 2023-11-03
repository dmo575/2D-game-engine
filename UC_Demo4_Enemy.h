#pragma once
#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Point, ColData
#include "Random.h"//For random object

class EC_Transform;
class EC_Sprite;
class EC_Collider;
class EC_Animator;
class UC_WASDController;

class UC_Demo4_Enemy : public Component
{
    friend class Engine;

public:
    UC_Demo4_Enemy(float vel, Point startPos, bool immortal = false);
    virtual ~UC_Demo4_Enemy() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void OnTriggerStay(Entity* other, ColData data) override;
    virtual void OnTriggerEnter(Entity* other, ColData data) override;

    void UpdateAnimationClip();
    void GetNewDirection();
    void UpdateMovement(float delta);
    void UpdateTurns();

private:
    EC_Transform* m_pTransform;
    EC_Animator* m_pAnimator;
    EC_Sprite* m_pSprite;
    EC_Collider* m_pCollider;
    std::unique_ptr<Random::RandomObject> randomObj;
    Point m_startPos;
    Point m_currDir;
    float m_countToDirChange;
    float m_vel;
    int m_turnsCounter;
    const int m_maxTurns;
    float m_timeCounter;
    const float m_maxTime;
    bool m_immortal;
};