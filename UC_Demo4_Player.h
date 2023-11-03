#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Point, ColData

class EC_Transform;
class EC_Sprite;
class EC_Collider;
class EC_Animator;
class EC_Sound;
class UC_WASDController;
class UC_MT_MovGridLocker;

class UC_Demo4_Player : public Component
{
public:
    UC_Demo4_Player(Point startPos);
    virtual ~UC_Demo4_Player() override;

    void AddHP(int value, bool respawn = false);
    int GetHP();
    void SetNewHP(int value);

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void OnTriggerEnter(Entity* other, ColData data) override;
    virtual void OnTriggerStay(Entity* other, ColData data) override;
    virtual void OnCollision(Entity* other, ColData data) override;

    void UpdateAnimationClip();

private:
    EC_Transform* m_pTransform;
    EC_Animator* m_pAnimator;
    EC_Sprite* m_pSprite;
    EC_Collider* m_pCollider;
    EC_Sound* m_pSound;
    UC_WASDController* m_pController;
    UC_MT_MovGridLocker* m_pMovLocker;
    Point m_startPos;
    Point m_lastMovingDir;
    int m_currHP;
    int m_maxHP;
    int m_keyCounter;
};