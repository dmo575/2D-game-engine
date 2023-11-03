#pragma once
#include "Component.h"

class EC_Transform;

class UC_Debug_Mover : public Component
{
public:
    UC_Debug_Mover(EC_Transform* t, Point dir, float vel);
    virtual ~UC_Debug_Mover() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Transform* m_t;
    Point m_dir;
    float m_vel;
};