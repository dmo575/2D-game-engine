#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Quad2D

class EC_Transform;
class EC_Sprite;

class UC_Demo5_Building : public Component
{
    friend class UC_Demo5_HallwayManager;

public:
    UC_Demo5_Building();
    virtual ~UC_Demo5_Building() override;

private:
    virtual void Init() override;

private:
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    Quad2D m_quadRaw;
};