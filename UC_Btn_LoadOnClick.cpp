#include <cstring>
#include "UC_Btn_LoadOnClick.h"
#include "UC_Includes.h"

UC_Btn_LoadOnClick::UC_Btn_LoadOnClick(const char* btnText, std::string sceneToLoad)
    : m_pTransform(nullptr)
    , m_pButton(nullptr)
    , m_btnTxt(btnText)
    , m_sceneToLoad(sceneToLoad.c_str())
{}

UC_Btn_LoadOnClick::~UC_Btn_LoadOnClick()
{}

void UC_Btn_LoadOnClick::Init()
{
    Rect btnRect{0,0,64,64};
    m_pButton = m_pParent->AddComponent(new EC_Button);

    m_pTransform = m_pParent->GetComponent<EC_Transform>();

    m_pButton->Sub(Event::CLICKABLE_CLICK_UP, this);
    m_pButton->SetPressImg("SP_Button.png", btnRect);
    m_pButton->SetLayer(10U);
    
    auto text = m_pParent->GetComponent<EC_Text>();
    text->SetFont("Fonts/arial.ttf", 12);
    text->SetText(m_btnTxt.c_str());
    text->SetColor(Color::PreDefined::k_Black);
}

void UC_Btn_LoadOnClick::Trigger(Event e, Entity* publisher)
{
    if (strcmp(m_sceneToLoad.c_str(), "cmnd_exit") == 0)
        GameManager::Get()->Quit();
    else
        LevelManager::Get()->QueueLoadLevel(m_sceneToLoad.c_str());
}
