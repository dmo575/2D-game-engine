#pragma once
#include "Component.h"//        Superclass

class UC_Demo_6_AnyKeyLoader : public Component
{
public:
    UC_Demo_6_AnyKeyLoader(const char* levelName);
    virtual ~UC_Demo_6_AnyKeyLoader() override;

private:
    virtual void Update(float delta) override;

private:
    const char* m_levelName;
};