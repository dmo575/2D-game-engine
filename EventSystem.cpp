#include "EventSystem.h"
#include "Component.h"
#include "Entity.h"

OEventSender::OEventSender(Entity* parent)
    : m_pParent(parent)
{
    m_pParent->GetSafeRef(&m_pParent);
}

OEventSender::~OEventSender()
{
    if(m_pParent)
        m_pParent->DestroySafeRef(&m_pParent);
}

void OEventSender::Sub(Event e, IEventListener* listener)
{
    //If the event is in the vec
    for (size_t i = 0; i < m_listeners.size(); ++i)
    {
        //Find event in vec
        if (m_listeners[i].m_eventType == e)
        {
            //If the listener was already registered:
            for (size_t listenerIndex = 0; listenerIndex < m_listeners[i].m_listeners.size(); ++listenerIndex)
            {
                if (m_listeners[i].m_listeners[listenerIndex] == listener)
                    return;
            }

            //If the listener was not registered yet.
            m_listeners[i].m_listeners.push_back(listener);
            return;
        }
    }

    //Reaching this means the event is NOT in the vector
    m_listeners.push_back(ListenerList(e));
    m_listeners[m_listeners.size() - 1].m_listeners.push_back(listener);
}

void OEventSender::Unsub(Event e, IEventListener* listener)
{
    for (size_t i = 0; i < m_listeners.size(); ++i)
    {
        //If we find the event in the vec
        if (m_listeners[i].m_eventType == e)
        {
            //Go trough all the listeners
            for (size_t listenerIndex = 0; listenerIndex < m_listeners[i].m_listeners.size(); ++listenerIndex)
            {
                //If we find the listener
                if (m_listeners[i].m_listeners[listenerIndex] == listener)
                {
                    //erase listener
                    m_listeners[i].m_listeners.erase(m_listeners[i].m_listeners.begin() + listenerIndex);

                    //If the event has no listeners left
                    if (m_listeners[i].m_listeners.size() == 0)
                    {
                        //erase event
                        m_listeners.erase(m_listeners.begin() + i);
                    }
                    return;
                }
            }
        }
    }

}

void OEventSender::TriggerEvent(Event e)
{
    for (size_t i = 0; i < m_listeners.size(); ++i)
    {
        //If we find the event
        if (m_listeners[i].m_eventType == e)
        {
            //Go trough all listeners
            for (size_t listenerIndex = 0; listenerIndex < m_listeners[i].m_listeners.size(); ++listenerIndex)
            {
                //Trigger event
                m_listeners[i].m_listeners[listenerIndex]->Trigger(e, m_pParent);
            }
            return;
        }
    }
}