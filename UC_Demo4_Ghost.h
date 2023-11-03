#pragma once
#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Point, ColData
#include "Random.h"//For random object
#include <vector>

class EC_Transform;
class EC_Sprite;
class EC_Collider;
class EC_Animator;
class UC_WASDController;

class UC_Demo4_Ghost : public Component
{
    friend class Engine;

public:
    UC_Demo4_Ghost(float vel, bool addCol = true);
    virtual ~UC_Demo4_Ghost() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

    void UpdateMovement(float delta);
    void UpdateAnimation();

private:
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    EC_Collider* m_pCollider;
    EC_Animator* m_pAnimator;
    std::unique_ptr<Random::RandomObject> randomObj;
    Point m_startPos;
    Point m_currDest;
    Point m_currDir;
    float m_vel;
    bool m_addCol;
};