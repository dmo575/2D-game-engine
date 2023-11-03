#include <cstring>
#include "UC_LevelLoader.h"
#include "UC_Includes.h"


UC_LevelLoader::UC_LevelLoader()
    : m_pPanel(nullptr)
    , m_pSprite(nullptr)
    , m_pPanelSprite(nullptr)
    , m_rectShow(0,0,512,512)
    , m_rectHide(0,0,0,0)
    , m_panelShowing(false)
    , m_currTrack(-1)
    , m_track1("TR_PQS7.mp3")
    , m_track2("TR_PQS7.mp3")
{}

UC_LevelLoader::~UC_LevelLoader()
{}

void UC_LevelLoader::Init()
{
    //---------------------SET UP SELF
    m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite);
    m_pSprite->SetSourceImg("TextImages/TI_Instructions.png", m_rectShow);

    m_pParent->DeleteOnLevelDestroy(false);


    //----------------------SET UP PANEL
    //(Although there's no need for safe ref in this case there's no harm in using it)
    m_pPanel =  m_entityManager->AddEntity("InfoPanel")->GetSafeRef(&m_pPanel);
    m_pPanel->AddComponent(new EC_Transform);
    m_pPanelSprite = m_pPanel->AddComponent(new EC_Sprite);

    m_pPanel->DeleteOnLevelDestroy(false);
    m_pPanelSprite->SetLayer(10);

    //Load track.
    if (!SoundManager::Get()->MusicPlaying())
    {
        SoundManager::Get()->PlayMusic(m_track1);
        m_currTrack = 1;
    }
}

void UC_LevelLoader::Update(float delta)
{
    if (InputModule::Get()->GetKeyState(KeyCode::k_Ctrl_L) == KeyState::k_KS_Hold)
    {
        UpdateLevelInfoInput();
        UpdateLevelLoadInput();
    }
    //If any key pressed and level is the introductory level, then go straight to Dare-Dungeon
    else if (InputModule::Get()->IsAnyKeyPressed(KeyState::k_KS_Press) &&
        InputModule::Get()->GetKeyState(KeyCode::k_Ctrl_L) != KeyState::k_KS_Press &&
        InputModule::Get()->GetKeyState(KeyCode::k_Ctrl_L) != KeyState::k_KS_Release &&
        strcmp(LevelManager::Get()->GetCurrLevel_Additive().c_str(), "demo_6_Instructions") == 0)
    {
        m_pSprite->SetSourceImg("", m_rectHide);
        m_pPanelSprite->SetSourceImg(GetDemoPanel(), m_rectHide);
        LevelManager::Get()->QueueLoadLevel("demo_4");
    }

    UpdateBGM();
}

void UC_LevelLoader::UpdateLevelLoadInput()
{
    if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_1) == KeyState::k_KS_Press ||
        InputModule::Get()->GetKeyState(KeyCode::k_Num_1) == KeyState::k_KS_Press)
    {
        m_pSprite->SetSourceImg("", m_rectHide);
        m_panelShowing = false;
        m_pPanelSprite->SetSourceImg(GetDemoPanel(), m_rectHide);
        LevelManager::Get()->QueueLoadLevel("demo_1");
    }
    else if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_2) == KeyState::k_KS_Press ||
        InputModule::Get()->GetKeyState(KeyCode::k_Num_2) == KeyState::k_KS_Press)
    {
        m_pSprite->SetSourceImg("", m_rectHide);
        m_pPanelSprite->SetSourceImg(GetDemoPanel(), m_rectHide);
        m_panelShowing = false;
        LevelManager::Get()->QueueLoadLevel("demo_2");
    }
    else if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_3) == KeyState::k_KS_Press ||
        InputModule::Get()->GetKeyState(KeyCode::k_Num_3) == KeyState::k_KS_Press)
    {
        m_pSprite->SetSourceImg("", m_rectHide);
        m_pPanelSprite->SetSourceImg(GetDemoPanel(), m_rectHide);
        m_panelShowing = false;
        LevelManager::Get()->QueueLoadLevel("demo_4");
    }
    else if (InputModule::Get()->GetKeyState(KeyCode::k_Keypad_4) == KeyState::k_KS_Press ||
        InputModule::Get()->GetKeyState(KeyCode::k_Num_4) == KeyState::k_KS_Press)
    {
        m_pSprite->SetSourceImg("", m_rectHide);
        m_pPanelSprite->SetSourceImg(GetDemoPanel(), m_rectHide);
        m_panelShowing = false;
        LevelManager::Get()->QueueLoadLevel("demo_5");
    }
}

void UC_LevelLoader::UpdateLevelInfoInput()
{
    if (InputModule::Get()->GetKeyState(KeyCode::k_I) == KeyState::k_KS_Press)
    {
        if (m_panelShowing)
            m_pPanelSprite->SetSourceImg(GetDemoPanel(), m_rectHide);
        else
            m_pPanelSprite->SetSourceImg(GetDemoPanel(), m_rectShow);

        m_panelShowing = !m_panelShowing;
    }

}

const char* UC_LevelLoader::GetDemoPanel()
{
    std::string currLevel = LevelManager::Get()->GetCurrLevel_Additive();

    if (currLevel == "demo_6_Instructions")
    {
        return "TextImages/TI_InformationLevelInfo.png";
    }
    else if (currLevel == "demo_1")
    {
        return "TextImages/TI_OriginalDemoInfo.png";
    }
    else if (currLevel == "demo_2")
    {
        return "TextImages/TI_SafeReferencesInfo.png";
    }
    else if (currLevel == "demo_4")
    {
        return "TextImages/TI_DareDungeonInfo.png";
    }
    else if (currLevel == "demo_5")
    {
        return "TextImages/TI_SandBoxDemoInfo.png";
    }
    return "";
}

void UC_LevelLoader::UpdateBGM()
{
    static SoundManager* manager = SoundManager::Get();

    if (!manager->MusicPlaying())
    {
        const char* trackToPlay = m_currTrack == 1 ? m_track2 : m_track1;
        m_currTrack = m_currTrack == 1 ? 2 : 1;

        manager->PlayMusic(trackToPlay);
    }
}