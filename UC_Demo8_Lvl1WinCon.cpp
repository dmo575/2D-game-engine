#include "UC_Demo8_Lvl1WinCon.h"
#include "UC_Includes.h"

UC_Demo8_Lvl1WinCon::UC_Demo8_Lvl1WinCon(EC_Transform* playerT, Point winPoint, float winPointRange)
    : m_playerT(playerT)
    , m_winPoint(winPoint)
    , m_winPointRange(winPointRange)
{}

UC_Demo8_Lvl1WinCon::~UC_Demo8_Lvl1WinCon()
{}

void UC_Demo8_Lvl1WinCon::Init()
{

}

void UC_Demo8_Lvl1WinCon::Update(float delta)
{
    if (MathModule::Distance(m_playerT->GetPos(), m_winPoint) < m_winPointRange)
        LevelManager::Get()->QueueLoadLevel("demo_2");
}