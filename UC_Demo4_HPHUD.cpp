#include "UC_Demo4_HPHUD.h"
#include "UC_Includes.h"
#include "UC_Demo4_Player.h"
#include <iostream>
#include <string>

UC_Demo4_HPHUD::UC_Demo4_HPHUD(Entity* player)
    : m_pPlayer(player)
    , m_pPlayerComp(nullptr)
    , m_pText(nullptr)
    , m_currHP(0)
{}

UC_Demo4_HPHUD::~UC_Demo4_HPHUD()
{}

void UC_Demo4_HPHUD::Init()
{
    //get safe ref to player ent
    m_pPlayer = m_pPlayer->GetSafeRef(&m_pPlayer);

    //if player ent is in memory, get its component
    if (m_pPlayer)
        m_pPlayerComp = m_pPlayer->GetComponent<UC_Demo4_Player>();

    //add a text component, this text comp will create a transform comp
    m_pText = m_pParent->AddComponent(new EC_Text);
    //get transform comp and edit its pos
    auto transform = m_pParent->GetComponent<EC_Transform>();
    transform->SetPos(12, 12);
    //set up text comp
    m_pText->SetColor(Color::PreDefined::k_Green);
    m_pText->SetFont("Fonts/arial.ttf", 20);

    //update local currHP and set text to display it
    m_currHP = m_pPlayerComp->GetHP();
    std::string s = std::to_string(m_currHP);
    m_pText->SetText(s.c_str());
}

void UC_Demo4_HPHUD::Update(float delta)
{
    //if player no longer exist, do not update
    if (!m_pPlayer) return;

    //if player's hp changes, update text UI
    if (m_currHP != m_pPlayerComp->GetHP())
    {
        m_currHP = m_pPlayerComp->GetHP();
        std::string s = std::to_string(m_currHP);
        m_pText->SetText(s.c_str());
    }
}