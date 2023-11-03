#include "Entity.h"
#include "EntityManager.h"
#include "Component.h"
#include <iostream>

EntityManager* Entity::m_pEntityManager = nullptr;

Entity::Entity(int id, std::string parentLevel)
    : Entity("Entity", id, parentLevel)
{}

Entity::Entity(const char* name, int id, std::string parentLevel)
    : m_name(name)
    , m_id(id)
    , m_destroyRequest(false)
    , m_deleteOnLevelDestroy(true)
    , m_parentLevel(parentLevel)
{}

Entity::~Entity()
{}

//Destroys the entity and its components
void Entity::Destroy()
{
    m_destroyRequest = true;

    VoidSafeReferences();

    for (size_t index = 0; index < m_components.size(); ++index)
    {
        delete m_components[index];
        m_components[index] = nullptr;
    }
}

bool Entity::GetDestroyRequest()
{
    return m_destroyRequest;
}

void Entity::Update(float delta)
{
    for (size_t i = 0; i < m_components.size() && !m_destroyRequest; ++i)
    {
        if(m_components[i]->IsActive())
            m_components[i]->Update(delta);
    }
}

//Returns NULL when entity is being destroyed
std::vector<Component*>* Entity::GetComponents()
{
    if (m_destroyRequest || m_components.size() == 0) return nullptr;
    return &m_components;
}

std::string Entity::GetName()
{
    return m_name;
}

//Returns true if the entity is being destroyed
bool Entity::IsBeingDestroyed()
{
    return m_destroyRequest;
}

//Returns NULL when entity is being destroyed
Entity* Entity::GetSafeRef(Entity** refereeAddress)
{
    if (m_destroyRequest) return nullptr;

    m_referees.push_back(&*refereeAddress);
    return this;
}

//Removes the safe reference from the referees vector
void Entity::DestroySafeRef(Entity** refereeAddress)
{
    for (size_t i = 0; i < m_referees.size(); ++i)
    {
        if (&*refereeAddress == &*m_referees[i])
        {
            m_referees.erase(m_referees.begin() + i);
            *refereeAddress = nullptr;
            return;
        }
    }
}

//NULLS all safe references from the referees vector
void Entity::VoidSafeReferences()
{
    for (size_t i = 0; i < m_referees.size(); ++i)
            *m_referees[i] = nullptr;
}

//Returns the entity's unique ID number
int Entity::GetID()
{
    return m_id;
}

void Entity::DeleteOnLevelDestroy(bool value)
{
    m_deleteOnLevelDestroy = value;
}

std::string Entity::GetParentLevel()
{
    return m_parentLevel;
}