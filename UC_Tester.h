#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Color, KeyCode
#include <vector>//             std::vector
#include "EventSystem.h"
#include <memory>

class EC_Transform;
class EC_Sprite;

class UC_Tester : public Component, IEventSender
{
public:
    UC_Tester(KeyCode deleteKey, Color c);
    virtual ~UC_Tester() override;

    void AddTesterToVector(UC_Tester* other);
    void Sub(Event e, IEventListener* listener);
    void Unsub(Event e, IEventListener* listener);

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    std::vector<Entity**> m_others;
    EC_Transform* m_pOtherTransform;
    KeyCode m_deleteKey;
    Color m_color;
    std::unique_ptr<OEventSender> m_pEventSender;
};