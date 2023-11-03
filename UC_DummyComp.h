#pragma once
#include "Component.h"//        Superclass

class UC_DummyComp : public Component
{
public:
    UC_DummyComp()
    : Component()
    {};
    virtual ~UC_DummyComp() override
    {};
};