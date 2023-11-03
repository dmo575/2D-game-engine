#pragma once
#include "Component.h"
#include <string>

class UC_Demo8_SaveManager : public Component
{
public:
    struct GameData
    {
    public:
        GameData(float sessionID)
            : m_currLevel("demo_1")
            , m_playTime(0.0f)
            , m_saveCount(0)
            , m_sessionID(sessionID)
        {}

    public:
        char m_currLevel[16];
        float m_playTime;
        float  m_saveCount;
        float m_sessionID;
    };

public:
    UC_Demo8_SaveManager();
    ~UC_Demo8_SaveManager();

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    void UpdateInput();
    void SaveLevelRequest(unsigned int saveSlot);
    void LoadLevelRequest(unsigned int loadSlot);
    void PrintSaveData(GameData data, std::string title);

private:
    GameData m_gameData;
    std::string m_saveFilesPath;
    std::string m_safeFile_1;
    std::string m_safeFile_2;
    std::string m_safeFile_3;
};