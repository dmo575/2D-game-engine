#include "GameManager.h"

//TEMPORAL UNTIL I MAKE THE APPMODULE OR OSMODULE
#include <SDL.h>

GameManager* GameManager::m_self = nullptr;

GameManager::GameManager()
    : m_deltaVel(1.0f)
    , m_quitRequest(false)
    , m_sessionTime(0.0f)
    , m_showCursor(true)
{
    Init();
}

GameManager::~GameManager()
{}

void GameManager::Init()
{
    if (!m_self)
    {
        m_self = this;
    }
}

float GameManager::GetSessionTime()
{
    return m_sessionTime;
}

void GameManager::SetDeltaVel(float value)
{
    if      (value > 1.0f) value = 1.0f;
    else if (value < 0.0f) value = 0.0f;

    m_deltaVel = value;
}

float GameManager::GetDeltaVel()
{
    return m_deltaVel;
}

void GameManager::Quit()
{
    m_quitRequest = true;
}

void GameManager::ShowCursor(bool value)
{
    m_showCursor = value;
    SDL_ShowCursor(value);
}

bool GameManager::GetShowCursor()
{
    return m_showCursor;
}