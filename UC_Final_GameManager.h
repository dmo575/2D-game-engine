#pragma once
#include "Component.h"
#include "EventSystem.h"
#include <string>

class EC_Transform;
class EC_Sprite;
class EC_Text;

class UC_Final_GameManager : public Component, IEventListener
{
public:
    struct SaveData
    {
    public:
        SaveData(time_t sessionStartTime)
            : m_currLevel("EMPTY")
            , m_playTime(0.0f)
            , m_saveCount(0)
            , m_sessionTime(sessionStartTime)
        {}

    public:
        char m_currLevel[16];
        float m_playTime;
        unsigned int  m_saveCount;
        time_t m_sessionTime;
    };

private:
    enum class MenuState { k_Closed, k_Pause, k_Save, k_Load, k_QuickSave, k_QuickLoad };
public:
    UC_Final_GameManager();
    virtual ~UC_Final_GameManager() override;
    virtual void Trigger(Event e, Entity* publisher) override;
    SaveData GetSelectedLevelData();

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

    void UpdateInput();
    void SetMenuState(MenuState newState);
    bool SaveRequest(unsigned int slotNumber);
    bool LoadRequest(unsigned int slotNumber, bool onlyUpdate = false);
    void TogglePanel(bool show);
    void WritePanelLine(std::string text);
    void WritePanelLine(unsigned int line, std::string text);
    void CheckLevelChange();
    void SpawnCursor();
    void UpdateBGM();

private:
    MenuState m_menuState;
    EC_Transform* m_pinfoPanelT;
    EC_Sprite* m_pInfoPanelS;
    EC_Transform* m_pInfoText1T;
    EC_Text* m_pInfoText1Text;
    EC_Transform* m_pInfoText2T;
    EC_Text* m_pInfoText2Text;
    EC_Transform* m_pInfoText3T;
    EC_Text* m_pInfoText3Text;
    EC_Transform* m_pCursorT;
    time_t m_gameSessionStartTime;
    float m_unsavedPlayTime;
    size_t m_currSlotIndex;
    SaveData m_saves[3];
    size_t m_hoveredLevelIndex;
    Point m_infoPanelPos;
    unsigned int m_maxCharsPerInfoLine;
    bool m_panelToggleState;
    std::string m_levelTracker;
    bool m_mouseStateInLevel;

    //demos' names
    const char* m_mainMenu;
    const char* m_lvl1;//og
    const char* m_lvl2;//numpad keys
    const char* m_lvl3;//dd
    const char* m_lvl4;//fps
    const char* m_lvl5;//flamethrower
    const char* m_lvl6;//key

    //information panel data
    const char* m_lvl1_about;
    const char* m_lvl1_objective;
    const char* m_lvl1_controls;
    const char* m_lvl2_about;
    const char* m_lvl2_objective;
    const char* m_lvl2_controls;
    const char* m_lvl3_about;
    const char* m_lvl3_objective;
    const char* m_lvl3_controls;
    const char* m_lvl4_about;
    const char* m_lvl4_objective;
    const char* m_lvl4_controls;
    const char* m_lvl5_about;
    const char* m_lvl5_objective;
    const char* m_lvl5_controls;
    const char* m_lvl6_about;
    const char* m_lvl6_objective;
    const char* m_lvl6_controls;
    int m_currTrack;
    const char* m_track1;
    const char* m_track2;

    //menus' names
    const char* m_pauseMenu;
    const char* m_saveLoadMenu;
};