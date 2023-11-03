#include "EventBus.h"
#include "LevelManager.h"
#include "Logger.h"

EventBus* EventBus::m_pSelf = nullptr;

EventBus::EventBus()
{
    Init();
}

EventBus::~EventBus()
{}

void EventBus::Init()
{
    if (!m_pSelf)
        m_pSelf = this;
}

void EventBus::QueueEvent(Event e)
{
    m_events.push_back(e);
}

void EventBus::ProcessEvents()
{
    for (size_t i = 0; i < m_events.size(); ++i)
    {
        switch (m_events[i])
        {
        case Event::k_Level_Load:
        {
            LevelManager::LoadData data = LevelManager::Get()->m_toLoad;
            LevelManager::Get()->LoadLevel(data.levelName, data.additive, data.deleteSafeEnts);
        }
            break;

        case Event::k_Level_Unload:
        {
            LevelManager::LoadData data = LevelManager::Get()->m_toUnload;
            LevelManager::Get()->UnloadLevel(data.levelName, data.deleteSafeEnts);
        }
        break;

        default:
            Logger::Log(Logger::LOG_LEVEL::k_Error, "EventBus is not finished yet. This should not be called");
            break;
        }

        m_events.erase(m_events.begin());
        i--;
    }
}