#pragma once
#include "Component.h"
#include "EventSystem.h"

class EC_Transform;
class UC_Tester;

class UC_Demo8_Lvl2WinCon : public Component, IEventListener
{
public:
    UC_Demo8_Lvl2WinCon();
    ~UC_Demo8_Lvl2WinCon();
    void AddTester(UC_Tester* tester);
    virtual void Trigger(Event e, Entity* publisher) override;

private:
    int m_keyMap[9];
};