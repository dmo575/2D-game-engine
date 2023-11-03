#include "UC_Demo8_SaveManager.h"
#include "UC_Includes.h"
#include "Random.h"
#include <iostream>

UC_Demo8_SaveManager::UC_Demo8_SaveManager()
    : m_gameData(Random::GetObject()->GetRandomValue(0.0f, 9999.0f))
    , m_saveFilesPath("Resources/Saves")
    , m_safeFile_1("demo8_saveData_1.save")
    , m_safeFile_2("demo8_saveData_2.save")
    , m_safeFile_3("demo8_saveData_3.save")
{

}

UC_Demo8_SaveManager::~UC_Demo8_SaveManager()
{}

void UC_Demo8_SaveManager::Init()
{
    m_pParent->DeleteOnLevelDestroy(false);
}

void UC_Demo8_SaveManager::Update(float delta)
{
    UpdateInput();
}

void UC_Demo8_SaveManager::UpdateInput()
{
    //If we are holding the Keft Ctrl key:
    if (InputModule::Get()->GetKeyState(KeyCode::k_I) == KeyState::k_KS_Hold)
    {
        if (InputModule::Get()->GetKeyState(KeyCode::k_Num_1) == KeyState::k_KS_Press ||
            InputModule::Get()->GetKeyState(KeyCode::k_Keypad_1) == KeyState::k_KS_Press)
            SaveLevelRequest(1);
        else if (InputModule::Get()->GetKeyState(KeyCode::k_Num_2) == KeyState::k_KS_Press ||
            InputModule::Get()->GetKeyState(KeyCode::k_Keypad_2) == KeyState::k_KS_Press)
            SaveLevelRequest(2);
        else if (InputModule::Get()->GetKeyState(KeyCode::k_Num_3) == KeyState::k_KS_Press ||
            InputModule::Get()->GetKeyState(KeyCode::k_Keypad_3) == KeyState::k_KS_Press)
            SaveLevelRequest(3);

    }
    else if (InputModule::Get()->GetKeyState(KeyCode::k_O) == KeyState::k_KS_Hold)
    {
        if (InputModule::Get()->GetKeyState(KeyCode::k_Num_1) == KeyState::k_KS_Press ||
            InputModule::Get()->GetKeyState(KeyCode::k_Keypad_1) == KeyState::k_KS_Press)
            LoadLevelRequest(1);
        else if (InputModule::Get()->GetKeyState(KeyCode::k_Num_2) == KeyState::k_KS_Press ||
            InputModule::Get()->GetKeyState(KeyCode::k_Keypad_2) == KeyState::k_KS_Press)
            LoadLevelRequest(2);
        else if (InputModule::Get()->GetKeyState(KeyCode::k_Num_3) == KeyState::k_KS_Press ||
            InputModule::Get()->GetKeyState(KeyCode::k_Keypad_3) == KeyState::k_KS_Press)
            LoadLevelRequest(3);
    }
}

void UC_Demo8_SaveManager::SaveLevelRequest(unsigned int saveSlot)
{
}

void UC_Demo8_SaveManager::LoadLevelRequest(unsigned int saveSlot)
{
}


void UC_Demo8_SaveManager::PrintSaveData(GameData data, std::string title)
{
    Logger::Log(std::string("\n"+ title + ": " +
    "\nLevel: " + data.m_currLevel +
    "\nPlay time: " + std::to_string(data.m_playTime) +
    "\nSave count: " + std::to_string(data.m_saveCount)).c_str());
}