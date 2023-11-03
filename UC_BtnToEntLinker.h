#pragma once
#include "Component.h"
#include "EventSystem.h"
#include <string>

class EC_Button;

class UC_BtnToEntLinker : public Component
{
public:
    UC_BtnToEntLinker();
    virtual ~UC_BtnToEntLinker() override;

    bool LinkBtnToEnt(std::string entName, EC_Button* btn, Event e);
};