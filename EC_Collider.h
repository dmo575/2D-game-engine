#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Rect
#include <vector>//             std::vector

class Entity;
class EC_Transform;
class EC_Sprite;
class ColliderManager;
class EntityManager;

class EC_Collider : public Component
{
    friend class ColliderManager;
    friend class EntityManager;

public:
    EC_Collider();
    virtual ~EC_Collider() override;

    void AttachToSprite(bool value);
    void SetColRect(Rect newColRect);
    const Rect GetColWorldRect();

    void SetTrigger(bool value);
    bool IsTrigger();

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

    void UpdateCollider(float delta);

    Point GetDirection();
    bool GetMoveState();
    void ResetMoveState();
    bool AddOverlap(Entity* overlap);
    void RemoveOverlap(Entity* overlap);
    std::vector<Entity*>* GetOverlaps();

private:
    static ColliderManager* m_pColManager;
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    std::vector<Entity*> m_overlaps;
    Rect m_colData;
    Point m_lastPos;
    Point m_currPos;
    bool m_isTrigger;
    bool m_moveState;
    bool m_useSpriteRect;
};