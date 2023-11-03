#pragma once
#include "Component.h"
#include "CoreStructs.h"

class EC_Sprite;
class EC_Transform;
class UC_Demo9_Flamethrower;

class UC_Demo9_FlamableObject : public Component
{
public:
    UC_Demo9_FlamableObject(UC_Demo9_Flamethrower* player, Point startPos);
    virtual ~UC_Demo9_FlamableObject() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    void UpdateObjectVisual(int mod);

private:
    static unsigned int m_instanceCounter;
    static unsigned int m_burntObjectCounter;
    UC_Demo9_Flamethrower* m_pPlayer;
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    Point m_startPos;
    float m_minDistForBurn;
    float m_heatNeededToMorph;
    float m_heatPoints;
    unsigned int m_morphCounter;
    float m_maxHeatInput;
};