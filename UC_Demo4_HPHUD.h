#pragma once
#include "Component.h"

class UC_Demo4_Player;
class Entity;
class EC_Text;

class UC_Demo4_HPHUD : public Component
{
public:
    UC_Demo4_HPHUD(Entity* player);
    virtual ~UC_Demo4_HPHUD() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    Entity* m_pPlayer;
    UC_Demo4_Player* m_pPlayerComp;
    EC_Text* m_pText;
    unsigned int m_currHP;
};