#include "UC_Demo_6_AnyKeyLoader.h"
#include "UC_Includes.h"

UC_Demo_6_AnyKeyLoader::UC_Demo_6_AnyKeyLoader(const char* levelName)
    : m_levelName(levelName)
{}

UC_Demo_6_AnyKeyLoader::~UC_Demo_6_AnyKeyLoader()
{}

void UC_Demo_6_AnyKeyLoader::Update(float delta)
{
    if (InputModule::Get()->IsAnyKeyPressed(KeyState::k_KS_Press))
    {
       LevelManager::Get()->QueueLoadLevel("demo_6_Instructions", false, true);
    }
}