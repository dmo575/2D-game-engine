#include <cstring>
#include "UC_Final_GameManager.h"
#include "UC_Includes.h"
#include <ctime>

UC_Final_GameManager::UC_Final_GameManager()
    : m_menuState(MenuState::k_Closed)
    , m_pinfoPanelT(nullptr)
    , m_pInfoPanelS(nullptr)
    , m_pInfoText1T(nullptr)
    , m_pInfoText1Text(nullptr)
    , m_pInfoText2T(nullptr)
    , m_pInfoText2Text(nullptr)
    , m_pInfoText3T(nullptr)
    , m_pInfoText3Text(nullptr)
    , m_pCursorT(nullptr)
    , m_gameSessionStartTime(time(0))
    , m_unsavedPlayTime(0.0f)
    , m_currSlotIndex(-1)
    , m_saves{ m_gameSessionStartTime, m_gameSessionStartTime, m_gameSessionStartTime }
    , m_hoveredLevelIndex(0)
    , m_infoPanelPos(20, 20)
    , m_maxCharsPerInfoLine(82)
    , m_panelToggleState(false)
    , m_levelTracker("")
    , m_mouseStateInLevel(true)
    , m_mainMenu("demo_final_mainMenu")
    , m_lvl1("demo_1")//og
    , m_lvl2("demo_2")//keypad
    , m_lvl3("demo_4")//DD
    , m_lvl4("demo_7_ShooterTutorial")//FPS
    , m_lvl5("demo_9_lvl1")//flamethrower
    , m_lvl6("demo_9_lvl2")//key
    , m_pauseMenu("demo_final_pauseMenu")
    , m_saveLoadMenu("demo_final_saveLoadMenu")
    , m_lvl1_about("This is the original demo. The pause menu was inspired by it.")
    , m_lvl1_controls("Use WASD to move the cursor around.")
    , m_lvl1_objective("Take the cursor to the TOP LEFT corner of the screen.")
    , m_lvl2_about("It showcases safe references and post-frame rendering.")
    , m_lvl2_controls("Use the numeric keys to destroy the knights.")
    , m_lvl2_objective("Destroy all knights.")
    , m_lvl3_about("It showcases a small mini-game built with the project.")
    , m_lvl3_controls("Use WASD to move around.")
    , m_lvl3_objective("Collect the 2 keys, open the door and find the exit.")
    , m_lvl4_about("Showcasing shapes that help give the illusion of depth among other things.")
    , m_lvl4_controls("Aim with the mouse, use LMB to shoot.")
    , m_lvl4_objective("Defeat the Boss (The giant Orb-like enemy)")
    , m_lvl5_about("Showcases particles and the many properties you can set up on them.")
    , m_lvl5_controls("Aim with the mouse, HOLD LMB to use the flamethrower")
    , m_lvl5_objective("Overheat all the cubic objects.")
    , m_lvl6_about("Showcases the particle system. It also implements a mechanic -hold object-")
    , m_lvl6_controls("Aim with the mouse, grab holding LBM. Use the level Button !")
    , m_lvl6_objective("Open the chests (4) by dropping the key on top of them.")
    , m_currTrack(-1)
    , m_track1("TR_PQS7.mp3")
    , m_track2("TR_PQS7.mp3")
{}

UC_Final_GameManager::~UC_Final_GameManager()
{}

void UC_Final_GameManager::Init()
{
    Rect infoPanelSrcRect{0,0,475, 82};
    m_pParent->DeleteOnLevelDestroy(false);
    LevelManager::Get()->QueueLoadLevel("demo_final_mainMenu");

    //Info panel
    auto infoPanelEnt = m_entityManager->AddEntity("InfoPanelEnt");
    infoPanelEnt->DeleteOnLevelDestroy(false);
    m_pinfoPanelT = infoPanelEnt->AddComponent(new EC_Transform);
    m_pInfoPanelS = infoPanelEnt->AddComponent(new EC_Sprite);

    m_pInfoPanelS->SetSourceImg("SP_InfoPanel.png", infoPanelSrcRect);
    m_pInfoPanelS->SetLayer(1100U);
    m_pinfoPanelT->SetPos(m_infoPanelPos);

    //Info text 1
    Point infoTextPos{ m_infoPanelPos.x + 10, m_infoPanelPos.y + 10 };
    auto infoText1Ent = m_entityManager->AddEntity("InfoTextEnt");
    infoText1Ent->DeleteOnLevelDestroy(false);
    m_pInfoText1Text = infoText1Ent->AddComponent(new EC_Text);
    m_pInfoText1T = infoText1Ent->GetComponent<EC_Transform>();

    m_pInfoText1Text->SetFont("Fonts/arial.ttf", 12);
    m_pInfoText1Text->SetColor(Color::PreDefined::k_Black);
    m_pInfoText1Text->SetText(" ");
    m_pInfoText1T->SetPos(infoTextPos);

    //Info text 2
    auto infoText2Ent = m_entityManager->AddEntity("InfoTextEnt");
    infoText2Ent->DeleteOnLevelDestroy(false);
    m_pInfoText2Text = infoText2Ent->AddComponent(new EC_Text);
    m_pInfoText2T = infoText2Ent->GetComponent<EC_Transform>();

    m_pInfoText2Text->SetFont("Fonts/arial.ttf", 12);
    m_pInfoText2Text->SetColor(Color::PreDefined::k_Black);
    m_pInfoText2Text->SetText(" ");
    m_pInfoText2T->SetPos(m_pInfoText1T->GetPos() + Point{0, 20});

    //Info text 3
    auto infoText3Ent = m_entityManager->AddEntity("InfoTextEnt");
    infoText3Ent->DeleteOnLevelDestroy(false);
    m_pInfoText3Text = infoText3Ent->AddComponent(new EC_Text);
    m_pInfoText3T = infoText3Ent->GetComponent<EC_Transform>();

    m_pInfoText3Text->SetFont("Fonts/arial.ttf", 12);
    m_pInfoText3Text->SetColor(Color::PreDefined::k_Black);
    m_pInfoText3Text->SetText(" ");
    m_pInfoText3T->SetPos(m_pInfoText2T->GetPos() + Point{ 0, 20 });

    //Load save files, this will update the saves array, which will itself keep on updating
    //with each save we do in the play session so we will also have the m_saves up to date
    //for wathever we need it.
    LoadRequest(0, true);
    LoadRequest(1, true);
    LoadRequest(2, true);

    TogglePanel(false);

    //Load track.
    if (!SoundManager::Get()->MusicPlaying())
    {
        SoundManager::Get()->PlayMusic(m_track1);
        SoundManager::Get()->Music_AddVolume(-100);

        m_currTrack = 1;
    }
}

void UC_Final_GameManager::Update(float delta)
{
    //This works nicely because pausing will add 0.0f to this time so we actually get the
    //raw play time which is what we are interested in doing. If we want the actual delta
    //we could always add a fund to GameManager.cs like GetRawDelta or GetRealDelta that
    //returns the delta without being modified by the delta vel variable.
    m_unsavedPlayTime += delta;

    UpdateInput();
    CheckLevelChange();
    UpdateBGM();
}

void UC_Final_GameManager::UpdateInput()
{
    //Gets the current NON-ADDITIVE level.
    std::string currLevel = LevelManager::Get()->GetCurrLevel_NonAdditive();

    //List of levels that we are not authorized to use the menus/panels on.
    if (strcmp(currLevel.c_str(), m_mainMenu) == 0 ||
        strcmp(currLevel.c_str(), "demo_final_controls") == 0 ||
        strcmp(currLevel.c_str(), "demo_final_demos") == 0 ||
        strcmp(currLevel.c_str(), "demo_6_GameOver") == 0 ||
        strcmp(currLevel.c_str(), "demo_6_YouWin") == 0)
        return;

    //If we press the ESC key
    if (InputModule::Get()->GetKeyState(KeyCode::k_Esc) == KeyState::k_KS_Press)
    {
        //Update the menu state based on the key press and its previous state.
        switch (m_menuState)
        {
        case UC_Final_GameManager::MenuState::k_Closed:
        {
            SetMenuState(MenuState::k_Pause);
            break;
        }
        case UC_Final_GameManager::MenuState::k_Pause:
        {
            SetMenuState(MenuState::k_Closed);
            break;
        }
        case UC_Final_GameManager::MenuState::k_Save:
        {
            SetMenuState(MenuState::k_Pause);
            break;
        }
        case UC_Final_GameManager::MenuState::k_Load:
        {
            SetMenuState(MenuState::k_Pause);
            break;
        }
        case UC_Final_GameManager::MenuState::k_QuickSave:
        {
            SetMenuState(MenuState::k_Closed);
            break;
        }
        case UC_Final_GameManager::MenuState::k_QuickLoad:
        {
            SetMenuState(MenuState::k_Closed);
            break;
        }
        }
    }//If we are holdint left Ctrl:
    else if (InputModule::Get()->GetKeyState(KeyCode::k_Ctrl_L) == KeyState::k_KS_Hold &&
        m_menuState == MenuState::k_Closed)
    {
        //Open either quick save or quick load depending on key press.
        if (InputModule::Get()->GetKeyState(KeyCode::k_S) == KeyState::k_KS_Press)
            SetMenuState(MenuState::k_QuickSave);
        else if (InputModule::Get()->GetKeyState(KeyCode::k_L) == KeyState::k_KS_Press)
            SetMenuState(MenuState::k_QuickLoad);
    }//If we are pressing the number 1 (keypad or normal)
    else if (InputModule::Get()->GetKeyState(KeyCode::k_Num_1) == KeyState::k_KS_Press ||
             InputModule::Get()->GetKeyState(KeyCode::k_Keypad_1) == KeyState::k_KS_Press)
    {
        //Either save or load depending on menu state (menu state can tell if we are saving
        //or loading)
        if (m_menuState == MenuState::k_Save || m_menuState == MenuState::k_QuickSave)
        {
            SaveRequest(0);
            SetMenuState(MenuState::k_Closed);
        }
        else if (m_menuState == MenuState::k_Load || m_menuState == MenuState::k_QuickLoad)
        {
            LoadRequest(0);
            SetMenuState(MenuState::k_Closed);
        }
    }//Same as above but with number 2
    else if (InputModule::Get()->GetKeyState(KeyCode::k_Num_2) == KeyState::k_KS_Press ||
        InputModule::Get()->GetKeyState(KeyCode::k_Keypad_2) == KeyState::k_KS_Press)
    {
        if (m_menuState == MenuState::k_Save || m_menuState == MenuState::k_QuickSave)
        {
            SaveRequest(1);
            SetMenuState(MenuState::k_Closed);
        }
        else if (m_menuState == MenuState::k_Load || m_menuState == MenuState::k_QuickLoad)
        {
            LoadRequest(1);
            SetMenuState(MenuState::k_Closed);
        }
    }//Same as above but with number 3
    else if (InputModule::Get()->GetKeyState(KeyCode::k_Num_3) == KeyState::k_KS_Press ||
        InputModule::Get()->GetKeyState(KeyCode::k_Keypad_3) == KeyState::k_KS_Press)
    {
        if (m_menuState == MenuState::k_Save || m_menuState == MenuState::k_QuickSave)
        {
            SaveRequest(2);
            SetMenuState(MenuState::k_Closed);
        }
        else if (m_menuState == MenuState::k_Load || m_menuState == MenuState::k_QuickLoad)
        {
            LoadRequest(2);
            SetMenuState(MenuState::k_Closed);
        }
    }//If we are pressing the I key, it means we need to toggle the info panel ON/OFF
    else if (InputModule::Get()->GetKeyState(KeyCode::k_I) == KeyState::k_KS_Press &&
        m_menuState == MenuState::k_Closed)
    {
        TogglePanel(!m_panelToggleState);
    }

}

//Updates the menu state and does some things based on context.
void UC_Final_GameManager::SetMenuState(MenuState newState)
{
    switch (m_menuState)
    {
    case UC_Final_GameManager::MenuState::k_Pause:
    {
        LevelManager::Get()->QueueUnloadLevel(m_pauseMenu);
        break;
    }
    case UC_Final_GameManager::MenuState::k_Save:
    case UC_Final_GameManager::MenuState::k_QuickSave:
    case UC_Final_GameManager::MenuState::k_Load:
    case UC_Final_GameManager::MenuState::k_QuickLoad:
    {
        LevelManager::Get()->QueueUnloadLevel(m_saveLoadMenu);
        break;
    }
    }

    m_menuState = newState;

    switch (m_menuState)
    {
    case UC_Final_GameManager::MenuState::k_Pause:
    {
        LevelManager::Get()->QueueLoadLevel(m_pauseMenu, true);
        break;
    }
    case UC_Final_GameManager::MenuState::k_Save:
    case UC_Final_GameManager::MenuState::k_QuickSave:
    case UC_Final_GameManager::MenuState::k_Load:
    case UC_Final_GameManager::MenuState::k_QuickLoad:
    {
        LevelManager::Get()->QueueLoadLevel(m_saveLoadMenu, true);
        break;
    }
    }

    //Close the panel when we open the menu.
    if (m_menuState != MenuState::k_Closed)
    {
        TogglePanel(false);
        m_mouseStateInLevel = GameManager::Get()->GetShowCursor();
        GameManager::Get()->ShowCursor(true);
    }
    else
        GameManager::Get()->ShowCursor(m_mouseStateInLevel);

    //Pause the game if we are in the menus. (setting the vel to 0 will tell the components
    //that are set up for it that the game is effectively paused.)
    GameManager::Get()->SetDeltaVel(newState == MenuState::k_Closed ? 1.0f : 0.0f);
}

//Each time we load the menu levels, we link the manager with its buttons, so the manager
//knows when any button it cares about is pressed.
//Here the manager will identify the button and act appropiately.
void UC_Final_GameManager::Trigger(Event e, Entity* publisher)
{
    //If we are in the pause menu, we listen to the pause menu buttons
    if (m_menuState == MenuState::k_Pause)
    {
        //This check should never trigger since we are not subscribing to it but since I wont
        //infividually perform this check after this point I think its best to have it here.
        if (e != Event::CLICKABLE_CLICK_UP) return;

        //Determine button press and act accordingly.

        if (strcmp(publisher->GetName().c_str(),"Btn_Resume") == 0)
            SetMenuState(MenuState::k_Closed);
        else if (strcmp(publisher->GetName().c_str(), "Btn_Save") == 0)
            SetMenuState(MenuState::k_Save);
        else if (strcmp(publisher->GetName().c_str(), "Btn_Load") == 0)
            SetMenuState(MenuState::k_Load);
        else if (strcmp(publisher->GetName().c_str(), "Btn_QuitToMainMenu") == 0)
        {
            LevelManager::Get()->QueueLoadLevel(m_mainMenu);
            SetMenuState(MenuState::k_Closed);
        }
        else if (strcmp(publisher->GetName().c_str(), "Btn_Vol-") == 0)
        {
            if (SoundManager::Get()->MusicPlaying())
                SoundManager::Get()->Music_AddVolume(-10);
        }
        else if (strcmp(publisher->GetName().c_str(), "Btn_Vol+") == 0)
        {
            if (SoundManager::Get()->MusicPlaying())
                SoundManager::Get()->Music_AddVolume(+10);
        }

    }//If we are in either the save or quick save state
    else if (m_menuState == MenuState::k_Save || 
             m_menuState == MenuState::k_QuickSave)
    {
        //Here is where we update the level info on hover.

        int hover = -1;// -1 means no hover, 1, 2 or 3 means the hovered slot
        int save = -1;// -1 means no save, 1, 2 or 3 means we want to save in that slot
        if (publisher->GetName() == "Btn_SaveSlot1")
        {
            hover = e == Event::CLICKABLE_HOVER_IN ? 0 : -1;
            save = hover == -1 ? 0 : -1;
        }
        else if (publisher->GetName() == "Btn_SaveSlot2")
        {
            hover = e == Event::CLICKABLE_HOVER_IN ? 1 : -1;
            save = hover == -1 ? 1 : -1;
        }
        else if (publisher->GetName() == "Btn_SaveSlot3")
        {
            hover = e == Event::CLICKABLE_HOVER_IN ? 2 : -1;
            save = hover == -1 ? 2 : -1;
        }

        if (save >= 0)
        {
            SaveRequest(save);
            SetMenuState(MenuState::k_Closed);
        }
        else
            m_hoveredLevelIndex = hover >= 0 ? hover : m_hoveredLevelIndex;

    }//Same as before but with load and quick load
    else if (m_menuState == MenuState::k_Load ||
        m_menuState == MenuState::k_QuickLoad)
    {
        int hover = -1;
        int load = -1;
        if (publisher->GetName() == "Btn_SaveSlot1")
        {
            hover = e == Event::CLICKABLE_HOVER_IN ? 0 : -1;
            load = hover == -1 ? 0 : -1;
        }
        else if (publisher->GetName() == "Btn_SaveSlot2")
        {
            hover = e == Event::CLICKABLE_HOVER_IN ? 1 : -1;
            load = hover == -1 ? 1 : -1;
        }
        else if (publisher->GetName() == "Btn_SaveSlot3")
        {
            hover = e == Event::CLICKABLE_HOVER_IN ? 2 : -1;
            load = hover == -1 ? 2 : -1;
        }

        if (load >= 0)
        {
            LoadRequest(load);
            SetMenuState(MenuState::k_Closed);
        }
        else
            m_hoveredLevelIndex = hover >= 0 ? hover : m_hoveredLevelIndex;
    }
}

//We use this to load a file, we can either just load it and not act upon its data
//or load it and load the level.
//onlyUpdate means we only want to update our m_saves variable, which contains
//all the saves info. We use that to update info on hover, but we dont load on hover,
//we load at the beginning of the game and on each new save, that way we mantain the 
//m_saves updated while keeping the loading operations at a minimum.
bool UC_Final_GameManager::LoadRequest(unsigned int slotNumber, bool onlyUpdate)
{
    size_t index = static_cast<size_t>(slotNumber);
    std::string path = "Resources/Saves/";
    std::string fileName = "final_demo_save" + std::to_string(slotNumber) + ".save";

    if (!SaveManager::Get()->LoadData(path, fileName, &m_saves[index], sizeof(SaveData)))
        return false;

    if (!onlyUpdate)
    {
        m_currSlotIndex = slotNumber;
        m_unsavedPlayTime = 0.0f;
        LevelManager::Get()->QueueLoadLevel(m_saves[index].m_currLevel);
    }

    return true;
}

//When saving, we make sure to update the info we might have on the slot first and
//merge it properly.
bool UC_Final_GameManager::SaveRequest(unsigned int slotNumber)
{
    size_t index = static_cast<size_t>(slotNumber);
    std::string path = "Resources/Saves/";
    std::string fileName = "final_demo_save" + std::to_string(slotNumber) + ".save";

    //load file data, if any.
    LoadRequest(slotNumber, true);

    //If we have not loaded any slot, this is a new game
    if (m_currSlotIndex == static_cast<size_t>(-1))
    {
        m_saves[index].m_playTime = m_unsavedPlayTime;
        m_saves[index].m_saveCount = 1;
    }
    else//Else, it means we are playing on top of a load, we can get its data
    {
        m_saves[index].m_playTime = m_saves[m_currSlotIndex].m_playTime + m_unsavedPlayTime;
        m_saves[index].m_saveCount = m_saves[m_currSlotIndex].m_saveCount + 1;
    }

    m_unsavedPlayTime = 0.0f;
    m_currSlotIndex = index;

    m_saves[index].m_sessionTime = time(0);

    //update level
    std::string currLevel = LevelManager::Get()->GetCurrLevel_NonAdditive();
    size_t i = 0;
    while (currLevel[i] != NULL)
    {
        m_saves[index].m_currLevel[i] = currLevel[i];
        ++i;
    }
    m_saves[index].m_currLevel[i] = 0;

    //save the data
    return SaveManager::Get()->SaveData(path, fileName, &m_saves[index], sizeof(SaveData));
}

UC_Final_GameManager::SaveData UC_Final_GameManager::GetSelectedLevelData()
{
    return m_saves[m_hoveredLevelIndex];
}

//This toggles the Information panel ON or OFF
void UC_Final_GameManager::TogglePanel(bool show)
{
    m_panelToggleState = show;
    m_pInfoPanelS->SetActive(show);
    m_pInfoText1Text->SetActive(show);
    m_pInfoText2Text->SetActive(show);
    m_pInfoText3Text->SetActive(show);

    if (!show) return;

    std::string currLevel = LevelManager::Get()->GetCurrLevel_NonAdditive();

    std::string about = "ABOUT: ";
    std::string controls = "CONTROLS: ";
    std::string objective = "OBJ: ";

    if (strcmp(currLevel.c_str(), m_lvl1) == 0)
    {
        about += m_lvl1_about;
        controls += m_lvl1_controls;
        objective += m_lvl1_objective;
    }
    else if (strcmp(currLevel.c_str(), m_lvl2) == 0)
    {
        about += m_lvl2_about;
        controls += m_lvl2_controls;
        objective += m_lvl2_objective;

    }
    else if (strcmp(currLevel.c_str(), m_lvl3) == 0)
    {
        about += m_lvl3_about;
        controls += m_lvl3_controls;
        objective += m_lvl3_objective;
    }
    else if (strcmp(currLevel.c_str(), m_lvl4) == 0)
    {
        about += m_lvl4_about;
        controls += m_lvl4_controls;
        objective += m_lvl4_objective;
    }
    else if (strcmp(currLevel.c_str(), m_lvl5) == 0)
    {
        about += m_lvl5_about;
        controls += m_lvl5_controls;
        objective += m_lvl5_objective;
    }
    else if (strcmp(currLevel.c_str(), m_lvl6) == 0)
    {
        about += m_lvl6_about;
        controls += m_lvl6_controls;
        objective += m_lvl6_objective;
    }

    WritePanelLine(0, about);
    WritePanelLine(1, controls);
    WritePanelLine(2, objective);
}

//We can use this function to write in the information panel, we will use the three lines
//automatically when using this function, but this means we cannot specify the line ourselves.
void UC_Final_GameManager::WritePanelLine(std::string text)
{
    EC_Text* textComps[3]{m_pInfoText1Text, m_pInfoText2Text, m_pInfoText3Text };
    textComps[0]->SetText(" ");
    textComps[1]->SetText(" ");
    textComps[2]->SetText(" ");

    unsigned int loops = 0;

    float reminder = text.size() / (float)m_maxCharsPerInfoLine;
    loops = (unsigned int)reminder;

    if (reminder - (int)reminder > 0.0f)
        loops++;

    if (loops > 3)
    {
        loops = 3;
        Logger::Log(Logger::LOG_LEVEL::k_Warning, "The text passed to the ino panel is too long for it. Text will be cut.");
    }

    size_t startIndex = 0;
    std::string lineText;
    for (size_t i = 0; i < static_cast<size_t>(loops); ++i)
    {
        if(text.size() >= startIndex + static_cast<size_t>(m_maxCharsPerInfoLine))
            lineText = text.substr(startIndex, static_cast<size_t>(m_maxCharsPerInfoLine));
        else
            lineText = text.substr(startIndex);

        textComps[i]->SetText(lineText.c_str());
        startIndex += static_cast<size_t>(m_maxCharsPerInfoLine);
    }
}

//This will print as much as the text as possible in this line and only this line.
void UC_Final_GameManager::WritePanelLine(unsigned int line, std::string text)
{
    std::string finalText = text.size() > m_maxCharsPerInfoLine ? text.substr(0, m_maxCharsPerInfoLine) : text;
    EC_Text* textComps[3]{ m_pInfoText1Text, m_pInfoText2Text, m_pInfoText3Text };

    textComps[static_cast<size_t>(line)]->SetText(text.c_str());
}

//If the level changed, make sure to update what you need.
void UC_Final_GameManager::CheckLevelChange()
{
    //If the level changed
    if (strcmp(LevelManager::Get()->GetCurrLevel_NonAdditive().c_str(), m_levelTracker.c_str()) != 0)
    {
        m_levelTracker = LevelManager::Get()->GetCurrLevel_NonAdditive();

        //close the info panel
        TogglePanel(false);
    }
}

//Spawns a cursor, it works but I decided on using the mouse.
void UC_Final_GameManager::SpawnCursor()
{
    Rect spriteRect{0,0,16,16};
    auto cursorEnt = m_entityManager->AddEntity("CursorEnt");

    m_pCursorT = cursorEnt->AddComponent(new EC_Transform);
    m_pCursorT->SetScale(0.5f, 0.5f);
    cursorEnt->DeleteOnLevelDestroy(false);
    auto cursorSprite = cursorEnt->AddComponent(new EC_Sprite);

    cursorSprite->SetSourceImg("SP_Cursor.png", spriteRect);
    cursorSprite->SetPivotToCenter();
    cursorSprite->SetLayer(110U);
    GameManager::Get()->ShowCursor(false);
}

//Updates the background music, it loops 2 tracks, however I set both track to be the same
//music since its the only decent music I found, the music track is also 30m long.
void UC_Final_GameManager::UpdateBGM()
{
    static SoundManager* manager = SoundManager::Get();

    if (!manager->MusicPlaying())
    {
        const char* trackToPlay = m_currTrack == 1 ? m_track2 : m_track1;
        m_currTrack = m_currTrack == 1 ? 2 : 1;

        manager->PlayMusic(trackToPlay);
    }
}