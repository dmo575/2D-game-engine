#include "UC_Demo5_Building.h"
#include "UC_Includes.h"

UC_Demo5_Building::UC_Demo5_Building()
    : m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_quadRaw()
{}

UC_Demo5_Building::~UC_Demo5_Building()
{}

void UC_Demo5_Building::Init()
{
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite);

    //Square
    m_quadRaw.p[0] = { 0, 0 };
    m_quadRaw.p[1] = { 0, -30 };
    m_quadRaw.p[2] = { 30, -30 };
    m_quadRaw.p[3] = { 30, 0 };
}