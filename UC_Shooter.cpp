#include "UC_Shooter.h"
#include "UC_Includes.h"
#include "UC_Bulltet.h"

UC_Shooter::UC_Shooter()
    : m_spriteFileName("")
    , m_srcRect()
    , m_type(0)
    , m_vel(0.0f)
    , m_dmg(0.0f)
    , m_zDir(0)
    , m_bulletMinScale(1.0f)
    , m_bulletMaxScale(150.0f)
{}

UC_Shooter::~UC_Shooter()
{}

void UC_Shooter::Init()
{}

void UC_Shooter::Update(float delta)
{
    if (GameManager::Get()->GetDeltaVel() == 0.0f) return;
    UpdateMissles(delta);
}

void UC_Shooter::UpdateMissles(float delta)
{
    if (m_missles.size() == 0) return;

    for (size_t i = 0; i < m_missles.size(); ++i)
    {
        m_missles[i].m_counter += delta;

        if (m_missles[i].m_counter >= m_missles[i].m_ttw)
        {
            SpawnMissleDOWN(m_missles[i].m_startPos, m_missles[i].m_targetPos);
            m_missles.erase(m_missles.begin() + i);
            i--;
        }
    }
}

void UC_Shooter::SpawnBullet(Point startPos, Point destPos)
{
    auto bulletEnt = EntityManager::Get()->AddEntity("ENT_BULLET");
    auto bulletComp = bulletEnt->AddComponent(new UC_Bullet(m_type, startPos, destPos, m_spriteFileName.c_str(), m_srcRect, m_vel, m_dmg, m_zDir, m_bulletMinScale, m_bulletMaxScale));

    for (auto name : m_targetNames)
    {
        bulletComp->AddTarget(name.c_str());
    }
}

void UC_Shooter::SpawnMissleUP(Point startPos, Point destPos, float ttw)
{
    Point upSpawnPoint{static_cast<float>(startPos.x), -550.0f};

    //spawn UP missle
    auto missleUpEnt = EntityManager::Get()->AddEntity("ENT_MISSLE_UP");
    auto projUpComp = missleUpEnt->AddComponent(new UC_Bullet(m_type, startPos, upSpawnPoint, m_spriteFileName.c_str(), m_srcRect, m_vel, m_dmg, m_zDir, m_bulletMinScale, m_bulletMinScale));

    m_missles.push_back(MissleDownData(upSpawnPoint, destPos, ttw));
}

void UC_Shooter::SpawnMissleDOWN(Point startPos, Point destPos)
{
    //spawn DOWN missle
    auto missleDownEnt = EntityManager::Get()->AddEntity("ENT_MISSLE_DOWN");
    auto projDownComp = missleDownEnt->AddComponent(new UC_Bullet(m_type, startPos, destPos, m_spriteFileName.c_str(), m_srcRect, m_vel, m_dmg, m_zDir, m_bulletMaxScale, m_bulletMaxScale));

    for (auto name : m_targetNames)
    {
        projDownComp->AddTarget(name.c_str());
    }
}


void UC_Shooter::SetProjectile(unsigned int bulletType, const char* spriteFileName, Rect srcRect, float vel, float dmg, int zDir, float minScale, float maxScale)
{
    m_type = bulletType;
    m_spriteFileName = spriteFileName;
    m_srcRect = srcRect;
    m_vel = vel;
    m_dmg = dmg;
    m_zDir = zDir;
    m_bulletMinScale = minScale;
    m_bulletMaxScale = maxScale;
}

void UC_Shooter::AddTarget(const char* targetEntName)
{
    m_targetNames.push_back(targetEntName);
}