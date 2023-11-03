#pragma once
#include "Component.h"
#include <string>

class EC_Transform;
class EC_Sprite;
class EC_Animator;

class UC_ExplodingEffect : public Component
{
public:
    UC_ExplodingEffect(const char* spriteName, Rect spriteSrcRect, unsigned int anmLength, float anmSpeed, Point spawnPos, float scale);
    virtual ~UC_ExplodingEffect() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    EC_Animator* m_pAnimator;
    std::string m_spriteName;
    Rect m_spriteSrcRect;
    unsigned int m_anmLength;
    float m_anmSpeed;
    Point m_spawnPos;
    float m_scale;
    float m_lifeSpam;
    float m_lifeSpamCounter;
};