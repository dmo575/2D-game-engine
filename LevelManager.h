#pragma once
#include <vector>//         std::vector
#include <functional>//     std::function
#include <string>

class EntityManager;
class RendererModule;
class ResourceModule;

class LevelManager
{
    friend class Engine;
    friend class EventBus;
private:
    struct Level
    {
    public:
        Level(std::string name, std::function<void()> levelFunc)
            : m_name(name)
            , m_levelFunc(levelFunc)
        {}
        ~Level(){};

    public:
        std::string m_name;
        std::function<void()> m_levelFunc;
    };

    struct LoadData
    {
        LoadData(const char* name, bool ad, bool delEnts)
            : levelName(name)
            , additive(ad)
            , deleteSafeEnts(delEnts)
        {}
        const char* levelName;
        bool additive;
        bool deleteSafeEnts;
    };

public:
    static LevelManager* Get()
    {
        return m_self;
    }

    std::string GetCurrLevel_Additive();
    std::string GetCurrLevel_NonAdditive();

    void QueueLoadLevel(const char* levelName, bool additive = false, bool deleteSafeEntities = false);
    void QueueUnloadLevel(const char* levelName, bool deleteSafeEntities = false);

private:
    LevelManager(RendererModule* ren, EntityManager* entityManager, ResourceModule* resourceModule);
    ~LevelManager();
    void Init();
    void AddLevel(std::string, std::function<void()> levelFunc);

    void LoadLevel(std::string levelName, bool additive = false, bool deleteSafeEntities = false);
    void UnloadLevel(std::string levelName, bool deleteSafeEntities = false);
private:
    static LevelManager* m_self;
    RendererModule* m_pRenderer;
    EntityManager* m_pEntityManager;
    ResourceModule* m_pResourceModule;
    std::vector<Level> m_levels;
    std::string m_currLevel_nonAdditive;
    std::string m_currLevel_additive;
    LoadData m_toLoad;
    LoadData m_toUnload;
};