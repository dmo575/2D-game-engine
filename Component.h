#pragma once
#include "CoreStructs.h"//      ColData


class Entity;
class EntityManager;
class ColliderManager;
class Component
{
    friend class EntityManager;
    friend class ColliderManager;
    friend class Entity;
public:
    Entity* GetParent();
    void SetActive(bool value);
    bool IsActive();

protected:
    Component();
    virtual ~Component();
    virtual void Init() {};
    virtual void Update(float delta) {};
    virtual void OnTriggerEnter(Entity* other, ColData data) {};
    virtual void OnTriggerStay(Entity* other, ColData data) {};
    //You can receive an Entity that is BEING DESTROYED.
    virtual void OnTriggerExit(Entity* other, ColData data) {};
    virtual void OnCollision(Entity* other, ColData data) {};

protected:
    static EntityManager* m_entityManager;
    Entity* m_pParent;
    int m_entityID;
    bool m_isActive;
};