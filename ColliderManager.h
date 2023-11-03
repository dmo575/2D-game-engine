#pragma once
#include "CoreStructs.h"//      Point, Rect
#include <vector>//             std::vector

class EC_Collider;
struct RenderableObject;
class Entity;

class ColliderManager
{
    friend class Engine;
    friend class EC_Collider;
public:
    enum class ColType {k_Collision, k_Trigger, k_All};
private:
    enum class ColFunc{k_OnCollision, k_OnTriggerEnter, k_OnTriggerStay, k_OnTriggerExit};
public:
    static ColliderManager* Get()
    {
        return m_pSelf;
    }
    bool IsPointOverlapping(Point point, ColType colType = ColType::k_All);
    bool IsPointOverlapping(Point point, std::vector<Entity*>* outOverlapEnts, ColType colType = ColType::k_All,  bool checkColToo = true);
    bool IsRectOverlapping(Rect rect, ColType colType = ColType::k_All);
    bool IsRectOverlapping(Rect rect, std::vector<Entity*>* outOverlapEnts, ColType colType = ColType::k_All, bool checkColToo = true);

private:
    ColliderManager();
    ~ColliderManager();
    void Init();
    void UpdateCollisions();
    void UpdateColliders(float delta);

    //Called by: EC_COLLIDER - Adds/Remove col to vec
    void AddCollider(EC_Collider* col);
    void DestroyCollider(EC_Collider* col);

    std::vector<RenderableObject*>* GetRenderableObjects();
    size_t GetRenderableObjectsSize();
    bool AABBCheck(const Rect& a, const Rect& b);
    bool HorizontalCheck(const Rect& a, const Rect& b);
    bool VerticalCheck(const Rect& a, const Rect& b);

    //These assume you already checked BOTH of these entities are not deleted.
    void CallColFunc(Entity* thisEnt, Entity* otherEnt, Point otherDir, ColFunc func);


private:
    static ColliderManager* m_pSelf;
    std::vector<EC_Collider*> m_Colliders;
    std::vector<RenderableObject*> m_colliderObjects;
    bool m_renderColliders;
};