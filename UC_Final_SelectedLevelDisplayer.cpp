#include <cstring>
#include "UC_Final_SelectedLevelDisplayer.h"
#include "UC_Includes.h"
#include "UC_Final_GameManager.h"
#include <string>

UC_Final_SelectedLevelDisplayer::UC_Final_SelectedLevelDisplayer(Point pos)
    : m_pGameManager(nullptr)
    , m_pLVL_txt(nullptr)
    , m_pLevel_txt(nullptr)
    , m_pTIME_txt(nullptr)
    , m_pTime_txt(nullptr)
    , m_pSAVES_txt(nullptr)
    , m_pSaves_txt(nullptr)
    , m_pivotPos(pos)
{}

UC_Final_SelectedLevelDisplayer::~UC_Final_SelectedLevelDisplayer()
{}

void UC_Final_SelectedLevelDisplayer::Init()
{
    //This will loop trough the entities and get the GameManager we instantiated back in the
    //pre-load scene. Because the GameManager was spawned in another scene, we can only get
    //a reference to it by looking for it by name.
    auto ents = m_entityManager->GetEntities();
    for (auto e : ents)
    {
        if (strcmp(e->GetName().c_str(), "GameManager") == 0)
        {
            m_pGameManager = e->GetComponent<UC_Final_GameManager>();
            break;
        }
    }

    Point LVLPos{m_pivotPos};
    Point levelPos{ LVLPos    + Point{0, 12} };
    Point TIMEPos{ levelPos   + Point{0, 17} };
    Point timePos{ TIMEPos    + Point{0, 12} };
    Point SAVESPos{ timePos   + Point{0, 17} };
    Point savesPos{ SAVESPos  + Point{0, 12} };


    //TEXT - LVL:
    m_pLVL_txt = m_pParent->AddComponent(new EC_Text);
    m_pLVL_txt->SetText("LVL:");
    m_pLVL_txt->SetColor(Color::PreDefined::k_Black);
    m_pLVL_txt->SetFont("Font/arial.ttf", 12);
    m_pParent->GetComponent<EC_Transform>()->SetPos(LVLPos);

    //TEXT - <level_name>
    auto levelEnt = m_entityManager->AddEntity("LevelInfoDIsplayer_SubEnt", m_pParent->GetParentLevel());
    m_pLevel_txt = levelEnt->AddComponent(new EC_Text);
    m_pLevel_txt->SetText(".");
    m_pLevel_txt->SetColor(Color::PreDefined::k_Black);
    m_pLevel_txt->SetFont("Fonts/arial.ttf", 12);
    levelEnt->GetComponent<EC_Transform>()->SetPos(levelPos);

    //TEXT - TIME:
    auto TIMEEnt = m_entityManager->AddEntity("LevelInfoDIsplayer_SubEnt", m_pParent->GetParentLevel());
    m_pTIME_txt = TIMEEnt->AddComponent(new EC_Text);
    m_pTIME_txt->SetText("TIME:");
    m_pTIME_txt->SetColor(Color::PreDefined::k_Black);
    m_pTIME_txt->SetFont("Fonts/arial.ttf", 12);
    TIMEEnt->GetComponent<EC_Transform>()->SetPos(TIMEPos);

    //TEXT - <time>
    auto timeEnt = m_entityManager->AddEntity("LevelInfoDIsplayer_SubEnt", m_pParent->GetParentLevel());
    m_pTime_txt = timeEnt->AddComponent(new EC_Text);
    m_pTime_txt->SetText(".");
    m_pTime_txt->SetColor(Color::PreDefined::k_Black);
    m_pTime_txt->SetFont("Fonts/arial.ttf", 12);
    timeEnt->GetComponent<EC_Transform>()->SetPos(timePos);

    //TEXT - SAVES:
    auto SAVESEnt = m_entityManager->AddEntity("LevelInfoDIsplayer_SubEnt", m_pParent->GetParentLevel());
    m_pSAVES_txt = SAVESEnt->AddComponent(new EC_Text);
    m_pSAVES_txt->SetText("SAVES:");
    m_pSAVES_txt->SetColor(Color::PreDefined::k_Black);
    m_pSAVES_txt->SetFont("Fonts/arial.ttf", 12);
    SAVESEnt->GetComponent<EC_Transform>()->SetPos(SAVESPos);

    //TEXT - <saves>
    auto savesEnt = m_entityManager->AddEntity("LevelInfoDIsplayer_SubEnt", m_pParent->GetParentLevel());
    m_pSaves_txt = savesEnt->AddComponent(new EC_Text);
    m_pSaves_txt->SetText(".");
    m_pSaves_txt->SetColor(Color::PreDefined::k_Black);
    m_pSaves_txt->SetFont("Fonts/arial.ttf", 12);
    savesEnt->GetComponent<EC_Transform>()->SetPos(savesPos);


}

void UC_Final_SelectedLevelDisplayer::Update(float delta)
{
    const char* level = m_pGameManager->GetSelectedLevelData().m_currLevel;
    std::string playTime = std::to_string((int)m_pGameManager->GetSelectedLevelData().m_playTime);
    std::string saves = std::to_string(m_pGameManager->GetSelectedLevelData().m_saveCount);

    if(strcmp(m_pLevel_txt->GetText(), level) != 0)
        m_pLevel_txt->SetText(level);

    if (strcmp(m_pTime_txt->GetText(), playTime.c_str()) != 0)
        m_pTime_txt->SetText(playTime.c_str());

    if (strcmp(m_pSaves_txt->GetText(), saves.c_str()) != 0)
        m_pSaves_txt->SetText(saves.c_str());
}