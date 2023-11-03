#pragma once
#include <vector>//         std::vector
#include <string>
class Entity;
class Engine;

class EntityManager
{
    friend class Engine;
    friend class Entity;
    friend class LevelManager;

public:
    static EntityManager* Get() { return m_self; }
    Entity* AddEntity(const char* name, bool parentToAdditiveLvl = false);
    Entity* AddEntity(const char* name, std::string parentLevel);

    std::vector<Entity*> GetEntities();

private:
    EntityManager();
    ~EntityManager();
    void Init();
    void DeInit();
    void Clear(const char* levelName = "_ALL_LEVELS", bool deleteSafeEntities = false);
    void DeleteEntity(Entity* ent);
    void UpdateEnts_Upd(float delta);
    void UpdateEnts_Del();
    void UpdateEnts_Add();

private:
    static EntityManager* m_self;
    std::vector<Entity*> m_entsToAdd;
    std::vector<Entity*> m_entsToDelete;
    std::vector<Entity*> m_entsToUpdate;
    std::vector<int> m_deletedIndexes;
    int m_entIDCounter;
};