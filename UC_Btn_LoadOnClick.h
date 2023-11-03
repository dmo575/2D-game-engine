#pragma once
#include "Component.h"
#include "EventSystem.h"
#include <string>

class EC_Transform;
class EC_Button;


class UC_Btn_LoadOnClick : public Component, IEventListener
{
public:
    UC_Btn_LoadOnClick(const char* btnText, std::string sceneToLoad);
    virtual ~UC_Btn_LoadOnClick() override;

    virtual void Trigger(Event e, Entity* publisher) override;

private:
    virtual void Init() override;

private:
    EC_Transform* m_pTransform;
    EC_Button* m_pButton;
    std::string m_sceneToLoad;
    std::string m_btnTxt;
};