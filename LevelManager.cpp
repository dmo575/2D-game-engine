#include <cstring>
#include "LevelManager.h"
#include "EntityManager.h"
#include "RendererModule.h"
#include "ResourceModule.h"
#include "CoreStructs.h"
#include "Logger.h"
#include "EventBus.h"

LevelManager* LevelManager::m_self = nullptr;

LevelManager::LevelManager(RendererModule* ren, EntityManager* entityManager, ResourceModule* resourceModule)
    : m_pRenderer(ren)
    , m_pEntityManager(entityManager)
    , m_pResourceModule(resourceModule)
    , m_currLevel_nonAdditive("NONE")
    , m_currLevel_additive("NONE")
    , m_toLoad("", false, false)
    , m_toUnload("", false, false)
{
    Init();
}

LevelManager::~LevelManager()
{}

void LevelManager::Init()
{
    if(!m_self)
        m_self = this;
}

void LevelManager::QueueLoadLevel(const char* levelName, bool additive, bool deleteSafeEntities)
{
    m_toLoad.levelName = levelName;
    m_toLoad.additive = additive;
    m_toLoad.deleteSafeEnts = deleteSafeEntities;
    EventBus::Get()->QueueEvent(EventBus::Event::k_Level_Load);
}

void LevelManager::QueueUnloadLevel(const char* levelName, bool deleteSafeEntities)
{
    m_toUnload.levelName = levelName;
    m_toUnload.deleteSafeEnts = deleteSafeEntities;
    EventBus::Get()->QueueEvent(EventBus::Event::k_Level_Unload);
}


void LevelManager::LoadLevel(std::string levelName, bool additive, bool deleteSafeEntities)
{
    void* image = m_pResourceModule->LoadTexture(std::string(m_pResourceModule->GetBasePath() + std::string("Resources/TextImages/TI_Loading.png")).c_str());
    Rect destRect = Rect{0, 0, 512, 512};

    if (!additive)
    {
        //Clear the screen
        m_pRenderer->ClearScreen();

        //Draw the loading icon
        m_pRenderer->DrawImage(image, destRect);

        //Clear the entityManager (this deletes the entities, which deletes the components, which
        //causes the images to get out of referees which deletes the images from memory)
        m_pEntityManager->Clear("_ALL_LEVELS", deleteSafeEntities);
    }


    //Find level and call its load function.
    for (size_t i = 0; i <= m_levels.size() - 1; ++i)
    {
        if (strcmp(m_levels[i].m_name.c_str(), levelName.c_str()) == 0)
        {
            if (!additive)//Just to keep track of non-additive current levels
                m_currLevel_nonAdditive = levelName;// Needs to happen first
            else
                m_currLevel_additive = levelName;

            m_levels[i].m_levelFunc();// Needs to happen second

            Logger::Log(Logger::LOG_LEVEL::k_Info, 
                std::string("Level \"" + std::string(levelName) + std::string("\" loaded")).c_str());
            return;
        }
    }

    //If we get here it means the level name we provided is incorrect.
    Logger::Log(Logger::LOG_LEVEL::k_Error, 
        std::string("Trying to load an unknown level \"" + std::string(levelName) + "\"").c_str());
}

void LevelManager::UnloadLevel(std::string levelName, bool deleteSafeEntities)
{
    m_pEntityManager->Clear(levelName.c_str(), deleteSafeEntities);

    Logger::Log(Logger::LOG_LEVEL::k_Info,
        std::string("Level \"" + std::string(levelName) + std::string("\" unloaded")).c_str());
}

void LevelManager::AddLevel(std::string name, std::function<void()> levelFunc)
{
    for (size_t i = 0; i < m_levels.size() - 1 && m_levels.size() > 0; ++i)
    {
        if (strcmp(m_levels[i].m_name.c_str(), name.c_str()) == 0)
        {
            Logger::Log(Logger::LOG_LEVEL::k_Error, 
                std::string("Can't have two levels with the same name \"" + std::string(name) + "\"").c_str());
            return;
        }
    }

    m_levels.push_back(Level(name, levelFunc));
}

std::string LevelManager::GetCurrLevel_Additive()
{
    return m_currLevel_additive;
}

std::string LevelManager::GetCurrLevel_NonAdditive()
{
    return m_currLevel_nonAdditive;
}