#include "UC_Demo4_EnemySpawner.h"
#include "UC_Includes.h"
#include "UC_Demo4_Enemy.h"

UC_Demo4_EnemySpawner::UC_Demo4_EnemySpawner(float spawnRatio)
    : m_pTransform(nullptr)
    , m_pSound(nullptr)
    , m_spawnRatio(spawnRatio)
    , m_spawnCounter(0.0f)
{}

UC_Demo4_EnemySpawner::~UC_Demo4_EnemySpawner()
{}

void UC_Demo4_EnemySpawner::Init()
{
    Rect doorSrcRect(16*7, 16*5, 16, 16);
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pTransform->SetScale(3, 3);
    m_pTransform->SetPos(16 * 14, 16 * 2);

    auto sprite = m_pParent->AddComponent(new EC_Sprite);
    sprite->SetSourceImg("TX_Door.png", doorSrcRect);
    sprite->SetPivotToCenter();

    m_pSound = m_pParent->AddComponent(new EC_Sound);
    m_pSound->SetSound("SN_SPAWN.ogg");
}

void UC_Demo4_EnemySpawner::Update(float delta)
{
    m_spawnCounter += delta;

    if (m_spawnCounter >= m_spawnRatio)
    {
        auto skeleton = EntityManager::Get()->AddEntity("Skeleton");
        skeleton->AddComponent(new UC_Demo4_Enemy(55.0f, m_pTransform->GetPos()));
        m_pSound->Play();
        m_spawnCounter = 0.0f;
    }
}