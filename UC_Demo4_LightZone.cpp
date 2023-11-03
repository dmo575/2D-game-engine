#include "UC_Demo4_LightZone.h"
#include "UC_Includes.h"

UC_Demo4_LightZone::UC_Demo4_LightZone(Entity* toFollow)
    : m_pTransform(nullptr)
    , m_pToFollow(toFollow)
    , m_toFollorTransform(nullptr)
{}

UC_Demo4_LightZone::~UC_Demo4_LightZone()
{}

void UC_Demo4_LightZone::Init()
{
    Rect srcRect(0, 0, 1024, 1024);
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pTransform->SetScale(1.35f, 1.35f);
    auto sprite = m_pParent->AddComponent(new EC_Sprite(10U));
    sprite->SetSourceImg("SP_LightZone.png", srcRect);
    sprite->SetPivotToCenter();

    if (m_pToFollow)
    {
        m_pToFollow = m_pToFollow->GetSafeRef(&m_pToFollow);
        m_toFollorTransform = m_pToFollow->GetComponent<EC_Transform>();
    }
}

void UC_Demo4_LightZone::Update(float delta)
{
    if (m_pToFollow)
        m_pTransform->SetPos(m_toFollorTransform->GetPos());
}