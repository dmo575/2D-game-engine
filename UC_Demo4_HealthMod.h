#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Point, ColData

class EC_Transform;
class EC_Collider;
class EC_Sound;
class UC_Demo4_Player;
class Entity;

class UC_Demo4_HealthMod : public Component
{
public:
    UC_Demo4_HealthMod(Point size, int hpMod, bool isWinArea);
    virtual ~UC_Demo4_HealthMod() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void OnTriggerEnter(Entity* other, ColData data) override;
    virtual void OnTriggerExit(Entity* other, ColData data) override;

private:
    EC_Transform* m_pTransform;
    EC_Collider* m_pCollider;
    EC_Sound* m_pSound;
    Entity* m_pPlayerEnt;
    UC_Demo4_Player* m_pPlayerComp;
    Point m_areaSize;
    int m_hpMod;
    bool m_playerInside;
    float m_timeCounter;
    bool m_isWinArea;
};