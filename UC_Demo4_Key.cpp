#include "UC_Demo4_Key.h"
#include "UC_Includes.h"

UC_Demo4_Key::UC_Demo4_Key(float x, float y)
    : m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_pCollider(nullptr)
    , m_pos(static_cast<int>(x), static_cast<int>(y))
{}

UC_Demo4_Key::~UC_Demo4_Key()
{}

void UC_Demo4_Key::Init()
{
    Rect keySrcRect(0, 0, 37, 16);
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pTransform->SetPos(m_pos.x, m_pos.y);
    m_pSprite= m_pParent->AddComponent(new EC_Sprite);
    m_pSprite->SetSourceImg("SP_Key.png", keySrcRect);
    m_pSprite->SetPivotToCenter();
    m_pCollider = m_pParent->AddComponent(new EC_Collider);
    m_pCollider->AttachToSprite(true);
    m_pCollider->SetTrigger(true);
}

void UC_Demo4_Key::Update(float delta)
{}