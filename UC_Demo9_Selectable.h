#pragma once
#include "Component.h"
#include "EventSystem.h"
class EC_Transform;
class EC_Sprite;
class EC_Button;
class Entity;

class EC_ParticleSpawner;

class UC_Demo9_Selectable : public Component, IEventListener
{
public:
    enum class SelectableState {k_OffGrab, k_OnGrab};

public:
    UC_Demo9_Selectable();
    virtual ~UC_Demo9_Selectable() override;
    SelectableState GetState();
    virtual void Trigger(Event e, Entity* publisher) override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Transform* m_pSpawnerTransform;
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    EC_ParticleSpawner* m_pSpawner;
    EC_Button* m_pInvertButton;
    bool m_invertValue;
    float m_selectionDistance;
    SelectableState m_state;
};