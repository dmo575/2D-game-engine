#pragma once
#include "Component.h"//        Superclass

class Entity;
class EC_Sprite;

class UC_LevelLoader : public Component
{
public:
    UC_LevelLoader();
    virtual ~UC_LevelLoader() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

    void UpdateLevelLoadInput();
    void UpdateLevelInfoInput();
    const char* GetDemoPanel();
    void UpdateBGM();

private:
    Entity* m_pPanel;
    EC_Sprite* m_pSprite;
    EC_Sprite* m_pPanelSprite;
    Rect m_rectShow;
    Rect m_rectHide;
    bool m_panelShowing;

    int m_currTrack;
    const char* m_track1;
    const char* m_track2;
};