#pragma once
#include <vector>

class EventBus
{
    friend class Engine;
public:
    enum class Event 
    {
        k_Something_That_Needs_No_Parameters,
        k_Level_Load,
        k_Level_Unload,
    };

public:
    EventBus();
    ~EventBus();

    static EventBus* Get()
    {
        return m_pSelf;
    }

    void QueueEvent(Event e);

private:
    void Init();
    void ProcessEvents();

private:
    static EventBus* m_pSelf;
    std::vector<Event> m_events;
};