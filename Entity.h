#pragma once
#include "Component.h"//        Component
#include <vector>//             std::vector
#include <string>//             std::string

class Component;
class EntityManager;

class Entity
{
    friend class Engine;
    friend class EntityManager;
    friend class SpriteManager;

public:
    void Destroy();
    bool IsBeingDestroyed();
    void DeleteOnLevelDestroy(bool value);
    std::string GetParentLevel();

    //Can't add on destroy.
    template<class CompT>
    CompT* AddComponent(CompT* compToAdd)
    {
        if (m_destroyRequest) return nullptr;

        //If Component Type already added, return nullptr.
        for (Component* c : m_components)
        {
            if (dynamic_cast<CompT*>(c))
                return nullptr;
        }

        compToAdd->m_entityID = m_id;
        compToAdd->m_pParent = this;
        m_components.push_back(compToAdd);

        dynamic_cast<Component*>(compToAdd)->Init();
        return compToAdd;
    }

    //Returns NULL on Entity destroy
    template<class CompT>
    CompT* GetComponent()
    {
        if (m_destroyRequest || m_components.size() == 0) return nullptr;

        for (Component* comp : m_components)
        {
            if (dynamic_cast<CompT*>(comp))
                return (CompT*)comp;
        }

        return nullptr;
    }

    std::vector<Component*>* GetComponents();
    std::string GetName();

    Entity* GetSafeRef(Entity** refAddress);
    void DestroySafeRef(Entity** refAddress);
    int GetID();
    bool GetDestroyRequest();

private:
    Entity(int id, std::string parentLevel);
    Entity(const char* name, int id, std::string parentLevel);
    ~Entity();

    void Update(float delta);
    void VoidSafeReferences();

private:
    static EntityManager* m_pEntityManager;
    std::vector<Entity**> m_referees;

    std::vector<Component*> m_components;
    std::string m_name;
    int m_id;
    bool m_destroyRequest;
    bool m_deleteOnLevelDestroy;
    std::string m_parentLevel;
};