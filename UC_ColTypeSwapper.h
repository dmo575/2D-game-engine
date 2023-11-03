#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      KeyCode

class EC_Collider;

class UC_ColTypeSwapper : public Component
{
public:
    UC_ColTypeSwapper(KeyCode swapKey);
    virtual ~UC_ColTypeSwapper() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    KeyCode m_swapKey;
    EC_Collider* m_col;
};