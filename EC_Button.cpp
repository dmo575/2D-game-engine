#include "EC_Button.h"
#include "EC_Includes.h"
#include "EC_Transform.h"
#include "EC_Sprite.h"
#include "EC_Text.h"

EC_Button::EC_Button(unsigned int layer, bool removeDefaultKeyPress)
    : Component()
    , m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_pText(nullptr)
    , m_pHL(nullptr)
    , m_pHLTransform(nullptr)
    , m_pHLSprite(nullptr)
    , m_spriteDim(0,0,0,0)
    , m_isHovering(false)
    , m_layer(layer)
{
    if (!removeDefaultKeyPress)
        m_validKeys.push_back(KeyCode::k_LMB);
}

EC_Button::~EC_Button()
{}

void EC_Button::Init()
{
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite);
    m_pText = m_pParent->AddComponent(new EC_Text);
    m_pSender = std::make_unique<OEventSender>(m_pParent);

    m_pHL = m_entityManager->AddEntity("Bt_HL", m_pParent->GetParentLevel());
    m_pHLTransform = m_pHL->AddComponent(new EC_Transform);
    m_pHLSprite = m_pHL->AddComponent(new EC_Sprite);

    m_pSprite->SetLayer(m_layer);

    Rect rectHL_normal(0, 0, 64, 32);
    m_pHLSprite->SetSourceImg("SP_ButtonHL.png", rectHL_normal);
    m_pHLSprite->SetLayer(m_layer, true);
    m_pHLSprite->SetActive(false);
}

void EC_Button::Update(float delta)
{
    static InputModule* input = InputModule::Get();

    UpdateHovering(input);
    UpdateButtonPress(input);
}

void EC_Button::UpdateHovering(InputModule*& input)
{
    Point mousePos = input->GetMousePos();
    Rect rect = m_pSprite->GetWorldRect();

    if (mousePos.x >= rect.x && mousePos.x <= rect.x + rect.w &&
        mousePos.y >= rect.y && mousePos.y <= rect.y + rect.h)
    {
        //Is hovering.
        if (!m_isHovering)
        {
            m_isHovering = true;
            m_pHLTransform->SetPos(m_pTransform->GetPos());
            m_pHLSprite->SetActive(true);
            m_pSender->TriggerEvent(Event::CLICKABLE_HOVER_IN);
        }
    }
    else if (m_isHovering)
    {
        //Is not hovering.
        m_isHovering = false;
        m_pHLSprite->SetActive(false);
        m_pSprite->SetSourceRect(0, 0, m_spriteDim.w, m_spriteDim.h / 2);
        m_pSender->TriggerEvent(Event::CLICKABLE_HOVER_OUT);
    }
}

void EC_Button::UpdateButtonPress(InputModule*& input)
{
    if (!m_isHovering) return;


    //For each key we accept, check if its pressed. (returns if it detects one)
    for (size_t i = 0; i < m_validKeys.size(); ++i)
    {
        if (input->GetKeyState(m_validKeys[i]) == KeyState::k_KS_Press)
        {
            m_pSprite->SetSourceRect(0, m_spriteDim.h/2, m_spriteDim.w, m_spriteDim.h / 2);
            m_pSender->TriggerEvent(Event::CLICKABLE_CLICK_DOWN);
            return;
        }
    }

    //For each key we accept, check if its released. (returns if it detecs one)
    for (size_t i = 0; i < m_validKeys.size(); ++i)
    {
        if (input->GetKeyState(m_validKeys[i]) == KeyState::k_KS_Release)
        {
            m_pSprite->SetSourceRect(0, 0, m_spriteDim.w, m_spriteDim.h / 2);
            m_pSender->TriggerEvent(Event::CLICKABLE_CLICK_UP);
            return;
        }
    }
}

void EC_Button::AddKeyToPress(KeyCode keyCode)
{
    //Check the key is not already in there
    for (size_t i = 0; i < m_validKeys.size(); ++i)
    {
        if (m_validKeys[i] == keyCode)
            return;
    }

    //Add it
    m_validKeys.push_back(keyCode);
}

void EC_Button::SetPressImg(const char* textureName, Rect& spriteDim)
{
    m_spriteDim = spriteDim;
    Rect rect(0, 0, m_spriteDim.w, spriteDim.h / 2);
    m_pSprite->SetSourceImg(textureName, rect);
}

void EC_Button::SetPos(Point& pos)
{
    m_pTransform->SetPos(pos);
}

void EC_Button::Sub(Event e, IEventListener* listener)
{
    m_pSender->Sub(e, listener);
}

void EC_Button::Unsub(Event e, IEventListener* listener)
{
    m_pSender->Unsub(e, listener);
}

void EC_Button::SetLayer(unsigned int layer)
{
    m_pSprite->SetLayer(layer);//This must happen second
    m_pHLSprite->SetLayer(layer, true);//this must happen first 
    //The reason is that when changing layers, the sprite will be added to the BOTTOM of that
    //layer, and we want the highlight to be on top of the button but in the same layer.
}

void EC_Button::UseSmallHL()
{
    Rect rectHL_small(0, 0, 32, 32);
    m_pHLSprite->SetSourceImg("SP_ButtonHL_small.png", rectHL_small);
}