#pragma once

class GameManager
{
    friend class Engine;

public:
    static GameManager* Get()
    {
        return m_self;
    }

    float GetSessionTime();
    void SetDeltaVel(float value);
    float GetDeltaVel();
    void ShowCursor(bool value);
    bool GetShowCursor();
    void Quit();

private:
    GameManager();
    ~GameManager();
    void Init();

private:
    static GameManager* m_self;
    float m_deltaVel;
    bool m_quitRequest;
    float m_sessionTime;
    bool m_showCursor;
};