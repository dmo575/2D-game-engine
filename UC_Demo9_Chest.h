#pragma once
#include "Component.h"

class EC_Transform;
class EC_Sprite;
class UC_Demo9_Selectable;

class UC_Demo9_Chest : public Component
{
public:
    UC_Demo9_Chest(UC_Demo9_Selectable* s, Point startPos);
    virtual ~UC_Demo9_Chest() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    void OpenChest();

private:
    static int m_totalChests;
    static int m_openedChests;
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    UC_Demo9_Selectable* m_pSelectable;
    EC_Transform* m_pSelectableTransform;
    bool m_opened;
    Point m_startPos;
    float m_interactionDist;
};