#include "UC_LoadListener.h"
#include "UC_Includes.h"

UC_LoadListener::UC_LoadListener(const char* level, IEventSender* sender, bool justQuit)
    : m_levelToLoad(level)
    , m_pSender(sender)
    , m_justQuit(justQuit)
{}

UC_LoadListener::~UC_LoadListener()
{}

void UC_LoadListener::Init()
{
    m_pSender->Sub(Event::CLICKABLE_CLICK_UP, this);
}

void UC_LoadListener::Trigger(Event e, Entity* source)
{
    if (e == Event::CLICKABLE_CLICK_UP && !m_justQuit)
        LevelManager::Get()->QueueLoadLevel(m_levelToLoad);
    else if (m_justQuit)
        GameManager::Get()->Quit();
}