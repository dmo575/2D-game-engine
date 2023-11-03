#pragma once
#include <vector>

//      EVENTS
enum class Event
{
    CLICKABLE_HOVER_IN,
    CLICKABLE_HOVER_OUT,
    CLICKABLE_CLICK_DOWN,
    CLICKABLE_CLICK_UP,

    SHOOTER_SHIELD_DAMAGED,
    SHOOTER_SHIELDPIECE_ARRIVED,
    SHOOTER_OBR_DESTROYED
};



//This passes a pointer to the Component that called the event.
class Entity;
class IEventListener;

//      EventSender INTERFACE. You must implement this on any component you wish to turn
//      into an event sender.
//      You use these as a wraper for OEventSender's functions.
class IEventSender
{
public:
    virtual void Sub(Event e, IEventListener* listener) = 0;
    virtual void Unsub(Event e, IEventListener* listener) = 0;
};

//      EventSender OBJECT. You must have an instance to an object of this type in any
//      Component that you wish to turn into an event sender.
class OEventSender
{
private:
    struct ListenerList
    {
    public:
        ListenerList(Event t)
            :m_eventType(t)
        {}

    public:
        Event m_eventType;
        std::vector<IEventListener*> m_listeners;
    };

public:
    OEventSender(Entity* parent);
    ~OEventSender();
    void Sub(Event e, IEventListener* listener);
    void Unsub(Event e, IEventListener* listener);
    void TriggerEvent(Event e);

private:
    Entity* m_pParent;
    std::vector<ListenerList> m_listeners;
};


//      EventListener INTERFACE. You must implement these in any component that you wish to
//      turn into an event listener.
class IEventListener
{
public:
    virtual void Trigger(Event e, Entity* publisher) = 0;
};