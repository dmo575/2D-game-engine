#include "UC_Demo4_Wall.h"
#include "UC_Includes.h"

UC_Demo4_Wall::UC_Demo4_Wall(Rect wallData)
    : m_wallData(wallData)
{}

UC_Demo4_Wall::~UC_Demo4_Wall()
{}

void UC_Demo4_Wall::Init()
{
    auto transform = m_pParent->AddComponent(new EC_Transform);
    auto sprite = m_pParent->AddComponent(new EC_Sprite);
    auto collider = m_pParent->AddComponent(new EC_Collider);

    Rect srcRect(16*3, 16*2, 16, 16);

    transform->SetPos(m_wallData.x, m_wallData.y);
    sprite->SetSourceImg("TX_Tile.png", srcRect);
    sprite->SetTile(true, m_wallData.w, m_wallData.h);

    collider->AttachToSprite(false);
    collider->SetTrigger(false);
    collider->SetColRect(Rect(0, 0, m_wallData.w * 16, m_wallData.h * 16));
}

void UC_Demo4_Wall::Update(float delta)
{
}