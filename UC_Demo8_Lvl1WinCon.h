#pragma once
#include "Component.h"
#include "CoreStructs.h"

class EC_Transform;

class UC_Demo8_Lvl1WinCon : public Component
{
public:
    UC_Demo8_Lvl1WinCon(EC_Transform* playerT, Point winPoint, float winPointRange);
    ~UC_Demo8_Lvl1WinCon();

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Transform* m_playerT;
    Point m_winPoint;
    float m_winPointRange;
};