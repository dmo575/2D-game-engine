#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Point

class EC_Transform;
class EC_Sprite;
class EC_Collider;

class UC_Demo4_Key : public Component
{
public:
    UC_Demo4_Key(float x, float y);
    virtual ~UC_Demo4_Key() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    EC_Collider* m_pCollider;
    Point m_pos;
};