#include "Component.h"

EntityManager* Component::m_entityManager = nullptr;

Component::Component()
    : m_pParent(nullptr)
    , m_entityID(-1)
    , m_isActive(true)
{}

Component::~Component()
{}

Entity* Component::GetParent()
{
    return m_pParent;
}

bool Component::IsActive()
{
    return m_isActive;
}

void Component::SetActive(bool value)
{
    m_isActive = value;
}