#pragma once
#include "Component.h"
#include "CoreStructs.h"
#include "Random.h"
#include <string>

class EC_Transform;
class EC_Sprite;
class EC_Animator;


class UC_Dust : public Component
{
public:
    UC_Dust(float initialScale, int scaleDir, const char* sprite, Rect spriteSrcRect, Point spawnPos, Point travelDir, float vel, float scaleMult, float lifeSpam, int layer);
    virtual ~UC_Dust() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    void UpdateDustPos(float delta);
    void UpdateLifeSpam(float delta);


private:
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    std::unique_ptr<Random::RandomObject> randomObj;
    Rect m_srcRect;
    Point m_spawnPos;
    Point m_travelDirOriginal;
    Point m_travelDirFinal;
    std::string m_spriteName;
    float m_initialScale;
    int m_scaleDir;
    float m_dustVel;
    float m_scaleMult;
    float m_lifeSpam;
    float m_timeCounter;
    int m_layer;
};