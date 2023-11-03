#include "UC_Demo5_HallwayManager.h"
#include "UC_Includes.h"
#include "UC_Demo5_Building.h"

UC_Demo5_HallwayManager::UC_Demo5_HallwayManager(Point angle)
    : m_pTransform(nullptr)
    , m_angle(angle)
{}

UC_Demo5_HallwayManager::~UC_Demo5_HallwayManager()
{}

void UC_Demo5_HallwayManager::Init()
{
    m_pTransform = m_pParent->AddComponent(new EC_Transform);

    auto building1 = m_entityManager->AddEntity("BuildingEnt");
    auto b1T = building1->AddComponent(new UC_Demo5_Building);

    auto building2 = m_entityManager->AddEntity("BuildingEnt");
    auto b2T = building2->AddComponent(new UC_Demo5_Building);


    m_buildings.push_back(b1T);
    m_buildings.push_back(b2T);
}

void UC_Demo5_HallwayManager::Update(float delta)
{
    for (size_t i = 0; i < m_buildings.size(); ++i)
    {
        m_buildings[i]->m_pTransform->AddPos(m_angle * 10.0f * delta);
    }
}