#pragma once
#include "Component.h"
#include "CoreStructs.h"
#include <string>
#include <vector>

class UC_Bullet;

class UC_Shooter : public Component
{
private:
    struct MissleDownData
    {
    public:
        MissleDownData(Point sPos, Point tPos, float ttw)
            : m_startPos(sPos)
            , m_targetPos(tPos)
            , m_ttw(ttw)
            , m_counter(0.0f)
        {}
    public:
        Point m_startPos;
        Point m_targetPos;
        float m_ttw;
        float m_counter;
    };
public:
    UC_Shooter();
    virtual ~UC_Shooter() override;

    void AddTarget(const char* targetEntName);
    void SetProjectile(unsigned int projType, const char* spriteFileName, Rect srcRect, float vel, float dmg, int zDir, float minScale, float maxScale);
    void SpawnBullet(Point startPos, Point destPos);
    void SpawnMissleUP(Point startPos, Point destPos, float ttw);
    void UpdateMissles(float delta);

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    void SpawnMissleDOWN(Point startPos, Point destPos);

private:
    std::vector<std::string> m_targetNames;
    std::vector<MissleDownData> m_missles;
    std::string m_spriteFileName;
    Rect m_srcRect;
    unsigned int m_type;
    float m_vel;
    float m_dmg;
    int m_zDir;
    float m_bulletMaxScale;
    float m_bulletMinScale;
};