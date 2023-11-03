#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Rect

class UC_Demo4_Wall : public Component
{
public:
    UC_Demo4_Wall(Rect wallData);
    virtual ~UC_Demo4_Wall() override;

private:
    virtual void Init();
    virtual void Update(float delta);

private:
    Rect m_wallData;
};