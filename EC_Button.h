#pragma once
#include "Component.h"
#include "EventSystem.h"
#include <vector>
#include <memory>

class EC_Transform;
class EC_Sprite;
class EC_Text;
class InputModule;
class Entity;

class EC_Button : public Component, public IEventSender
{
public:
    EC_Button(unsigned int layer = 0, bool removeDefaultKeyPress = false);
    virtual ~EC_Button() override;

    virtual void Sub(Event e, IEventListener* listener) override;
    virtual void Unsub(Event e, IEventListener* listener) override;
    void AddKeyToPress(KeyCode keyCode);
    void SetPressImg(const char* textureName, Rect& spriteDim);
    void SetPos(Point& pos);
    void SetLayer(unsigned int layer);
    void UseSmallHL();

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

    void UpdateHovering(InputModule*& input);
    void UpdateButtonPress(InputModule*& input);

private:
    std::vector<KeyCode> m_validKeys;
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    EC_Text* m_pText;
    Entity* m_pHL;
    EC_Transform* m_pHLTransform;
    EC_Sprite* m_pHLSprite;

    std::unique_ptr<OEventSender> m_pSender;
    Rect m_spriteDim;
    bool m_isHovering;
    unsigned int m_layer;
};