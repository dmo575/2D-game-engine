#include <cstring>
#include "EntityManager.h"
#include "Entity.h"
#include "Component.h"
#include "LevelManager.h"
#include "Logger.h"
#include <iostream>

EntityManager* EntityManager::m_self = nullptr;
EntityManager::EntityManager()
    : m_entIDCounter(0)
{
    Init();
}

EntityManager::~EntityManager()
{}

void EntityManager::Init()
{
    m_self = this;
    Entity::m_pEntityManager = this;
    Component::m_entityManager = this;
}

void EntityManager::DeInit()
{
    for (size_t i = 0; i < m_entsToUpdate.size(); ++i)
    {
        if(m_entsToUpdate[i] != nullptr)
            m_entsToUpdate[i]->Destroy();
    }
}

Entity* EntityManager::AddEntity(const char* name, bool parentToAdditiveLvl)
{
    std::string parentLvl = parentToAdditiveLvl == true ? LevelManager::Get()->GetCurrLevel_Additive() : LevelManager::Get()->GetCurrLevel_NonAdditive();
    
    m_entsToAdd.push_back(new Entity(name, m_entIDCounter, parentLvl));

    m_entIDCounter++;

    return m_entsToAdd[m_entsToAdd.size() - 1];
}

Entity* EntityManager::AddEntity(const char* name, std::string parentlevel)
{
    m_entsToUpdate.push_back(new Entity(name, m_entIDCounter, parentlevel));

    m_entIDCounter++;

    return m_entsToUpdate[m_entsToUpdate.size() - 1];
}


std::vector<Entity*> EntityManager::GetEntities()
{
    return m_entsToUpdate;
}

void EntityManager::UpdateEnts_Upd(float delta)
{
    for (size_t i = 0; i < m_entsToUpdate.size(); ++i)
    {
        if (m_entsToUpdate[i]->GetDestroyRequest())
        {
            //Reaching this means the entity is requesting to be deleted.

            //Add ent to toDelete ents
            m_entsToDelete.push_back(m_entsToUpdate[i]);
            //Remove ent from toUpdate ents
            m_entsToUpdate.erase(m_entsToUpdate.begin() + i);

            //decrement pointer since this vector got modified
            i--;
            continue;
        }
        else
            m_entsToUpdate[i]->Update(delta);
    }
}

void EntityManager::UpdateEnts_Del()
{
    for (size_t i = 0; i < m_entsToDelete.size(); ++i)
        delete m_entsToDelete[i];

    m_entsToDelete.clear();
}

void EntityManager::UpdateEnts_Add()
{
    for (size_t i = 0; i < m_entsToAdd.size(); ++i)
        m_entsToUpdate.push_back(m_entsToAdd[i]);

    m_entsToAdd.clear();
}


//Called in the Entity class
void EntityManager::DeleteEntity(Entity* ent)
{
    for (size_t i = 0; i < m_entsToUpdate.size(); ++i)
    {
        if (m_entsToUpdate[i] == ent)
        {
            delete ent;
            m_entsToUpdate[i] = nullptr;
            return;
        }
    }
    Logger::Log(Logger::LOG_LEVEL::k_Error, "EntManager tried to delete an entity that doesn't exist.");
}

//Destroys all entities and resets the id counter
void EntityManager::Clear(const char* levelName, bool deleteSafeEntities)
{
    //size_t entitiesIndex = -1;
    for (size_t i = 0; i < m_entsToUpdate.size() && m_entsToUpdate.size() > 0; ++i)
    {
        if (m_entsToUpdate[i] == nullptr)
        {
            m_entsToUpdate.erase(m_entsToUpdate.begin() + i);
            i--;
            continue;
        }

        if (m_entsToUpdate[i]->m_deleteOnLevelDestroy || deleteSafeEntities)
        {
            if (strcmp(levelName, "_ALL_LEVELS") == 0 ||
                strcmp(levelName, m_entsToUpdate[i]->GetParentLevel().c_str()) == 0)
            {
                m_entsToUpdate[i]->Destroy();
                m_entsToUpdate.erase(m_entsToUpdate.begin() + i);
                i--;
            }
        }
    }

    if(strcmp(levelName, "_ALL_LEVELS") == 0 && deleteSafeEntities)
        m_entIDCounter = 0;
}