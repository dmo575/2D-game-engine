#include "UC_Demo8_Lvl2WinCon.h"
#include "UC_Includes.h"
#include "UC_Tester.h"

UC_Demo8_Lvl2WinCon::UC_Demo8_Lvl2WinCon()
{}

UC_Demo8_Lvl2WinCon::~UC_Demo8_Lvl2WinCon()
{}

void UC_Demo8_Lvl2WinCon::AddTester(UC_Tester* tester)
{
    ((IEventSender*)tester)->Sub(Event::SHOOTER_OBR_DESTROYED, this);
}

void UC_Demo8_Lvl2WinCon::Trigger(Event e, Entity* publisher)
{
    if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_1) == KeyState::k_KS_Press)
        m_keyMap[0] = 1;
    if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_2) == KeyState::k_KS_Press)
        m_keyMap[1] = 1;
    if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_3) == KeyState::k_KS_Press)
        m_keyMap[2] = 1;
    if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_4) == KeyState::k_KS_Press)
        m_keyMap[3] = 1;
    if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_5) == KeyState::k_KS_Press)
        m_keyMap[4] = 1;
    if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_6) == KeyState::k_KS_Press)
        m_keyMap[5] = 1;
    if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_7) == KeyState::k_KS_Press)
        m_keyMap[6] = 1;
    if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_8) == KeyState::k_KS_Press)
        m_keyMap[7] = 1;
    if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_9) == KeyState::k_KS_Press)
        m_keyMap[8] = 1;

    for (size_t i = 0; i < 9; ++i)
    {
        if (m_keyMap[i] == 0)
            return;
    }

    LevelManager::Get()->QueueLoadLevel("demo_4");
}