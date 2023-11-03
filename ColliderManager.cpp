#include "ColliderManager.h"
#include "GlobalMacros.h"
#include "EntityManager.h"
#include "Entity.h"
#include "EC_Collider.h"
#include "EC_Transform.h"
#include "RenderableObjec.h"
#include <iostream>

ColliderManager* ColliderManager::m_pSelf = nullptr;

ColliderManager::ColliderManager()
    : m_renderColliders(false)
{
    Init();
}

ColliderManager::~ColliderManager()
{}

void ColliderManager::Init()
{
    if(EC_Collider::m_pColManager == nullptr)
        EC_Collider::m_pColManager = this;

    if (!m_pSelf)
        m_pSelf = this;
}


void ColliderManager::AddCollider(EC_Collider* col)
{
    m_Colliders.push_back(col);
    
#if DEBUG_DRAW_COLLIDERS
    m_colliderObjects.push_back(new RenderableObject(RenderableObjectType::k_Rect,
        col->GetColWorldRect(), Color(Color::PreDefined::k_Red)));
#endif
}


void ColliderManager::DestroyCollider(EC_Collider* col)
{
    for (size_t i = 0; i < m_Colliders.size(); ++i)
    {
        if (col == m_Colliders[i])
        {
            //Remove col from m_Colliders (set to NULLPTR)
            m_Colliders[i] = nullptr;
#if DEBUG_DRAW_COLLIDERS
            /*DEBUG*/delete m_colliderObjects[i];
            /*DEBUG*/m_colliderObjects[i] = nullptr;
#endif
            break;
        }
    }
}
void ColliderManager::UpdateColliders(float delta)
{
    for (size_t i = 0; i < m_Colliders.size(); ++i)
    {
        if(m_Colliders[i])
            m_Colliders[i]->UpdateCollider(delta);
    }
}

void ColliderManager::UpdateCollisions()
{
    //CHECKPOINT: M_COLLIDERS IS SAFE TO USE ANYWHERE ( CHECKS NULLS)
    for (size_t thisI = 0; thisI < m_Colliders.size(); ++thisI)
    {
        //We make sure that THISI is NOT a nullptr.
        if (m_Colliders[thisI] == nullptr)
        {
            m_Colliders.erase(m_Colliders.begin() + thisI);
#if DEBUG_DRAW_COLLIDERS
            /*DEBUG*/m_colliderObjects.erase(m_colliderObjects.begin() + thisI);// Debug
#endif
            continue;
        }

        //Now when editing m_colliders make sure you only set to NULL, do NOT erase().

#if DEBUG_DRAW_COLLIDERS
        /*DEBUG*/m_colliderObjects[thisI]->m_data = m_Colliders[thisI]->GetColWorldRect();
#endif

        //CHECKPOINT: THIS_Col - VALID.
        if (m_Colliders[thisI]->GetMoveState())
        {
#if DEBUG_DRAW_COLLIDERS
            //Set the MOVING COLOR
            if (m_Colliders[thisI]->IsTrigger())
                /*DEBUG*/m_colliderObjects[thisI]->m_color = Color(Color::PreDefined::k_Cyan);
            else
                /*DEBUG*/m_colliderObjects[thisI]->m_color = Color(Color::PreDefined::k_Orange);
#endif
            for (size_t otherI = 0; otherI < m_Colliders.size(); ++otherI)
            {
                if (m_Colliders[otherI] == nullptr) continue; // OTHER doesnt exist
                if (m_Colliders[thisI] == nullptr) break; // THIS got deleted while looping
                if (otherI == thisI ||(otherI < thisI &&
                    m_Colliders[otherI]->GetMoveState()))
                    continue; // OTHER and THIS are the same or other already called for both.

                //CHECKPOINT: THIS_Col - VALID *** OTHER_Col - VALID 

                Rect thisRect = m_Colliders[thisI]->GetColWorldRect();
                Rect otherRect = m_Colliders[otherI]->GetColWorldRect();

                //If both THIS and OTHER are COLLIDERS
                if (!m_Colliders[thisI]->IsTrigger() && !m_Colliders[otherI]->IsTrigger())
                {
                    Entity* thisEnt = m_Colliders[thisI]->GetParent()->GetSafeRef(&thisEnt);
                    Entity* otherEnt = m_Colliders[otherI]->GetParent()->GetSafeRef(&otherEnt);

                    auto thisTransform = thisEnt->GetComponent<EC_Transform>();

                    if(AABBCheck(thisRect, otherRect))
                    {
#if DEBUG_DRAW_COLLIDERS
                        //Set the ON COLLISION COLOR
                        /*DEBUG*/m_colliderObjects[thisI]->m_color = Color(Color::PreDefined::k_Yellow);
                        /*DEBUG*/m_colliderObjects[otherI]->m_color = Color(Color::PreDefined::k_White);
#endif
                        thisTransform->SetPos(m_Colliders[thisI]->m_lastPos);
                        m_Colliders[thisI]->m_currPos = m_Colliders[thisI]->m_lastPos;

                        CallColFunc(thisEnt, otherEnt, m_Colliders[otherI]->GetDirection(), ColliderManager::ColFunc::k_OnCollision);

                        if(otherEnt && thisEnt)
                            CallColFunc(otherEnt, thisEnt, m_Colliders[thisI]->GetDirection(), ColliderManager::ColFunc::k_OnCollision);
                    }

                    if(thisEnt)
                        thisEnt->DestroySafeRef(&thisEnt);
                    if(otherEnt)
                        otherEnt->DestroySafeRef(&otherEnt);

                }//If either THIS or OTHER is a TRIGGER
                else if (AABBCheck(thisRect, otherRect))
                {
#if DEBUG_DRAW_COLLIDERS
                    //Set the ON COLLISION COLOR
                    if (m_Colliders[thisI]->IsTrigger())
                        /*DEBUG*/m_colliderObjects[thisI]->m_color = Color(Color::PreDefined::k_Green);
                    else
                        /*DEBUG*/m_colliderObjects[thisI]->m_color = Color(Color::PreDefined::k_Yellow);
#endif
                    Entity* thisEnt = m_Colliders[thisI]->GetParent()->GetSafeRef(&thisEnt);
                    Entity* otherEnt = m_Colliders[otherI]->GetParent()->GetSafeRef(&otherEnt);

                    bool wasOtherIntruder = !m_Colliders[thisI]->AddOverlap(otherEnt);
                    bool wasThisIntruder = !m_Colliders[otherI]->AddOverlap(thisEnt);


                    //CHECKPOINT: EVERYTHING VALID.
                    if (wasOtherIntruder)
                        CallColFunc(thisEnt, otherEnt, m_Colliders[otherI]->GetDirection(), ColliderManager::ColFunc::k_OnTriggerStay);
                    else
                        CallColFunc(thisEnt, otherEnt, m_Colliders[otherI]->GetDirection(), ColliderManager::ColFunc::k_OnTriggerEnter);

                    if (thisEnt && otherEnt)
                    {
                        if (wasThisIntruder)
                            CallColFunc(otherEnt, thisEnt, m_Colliders[thisI]->GetDirection(), ColliderManager::ColFunc::k_OnTriggerStay);
                        else
                            CallColFunc(otherEnt, thisEnt, m_Colliders[thisI]->GetDirection(), ColliderManager::ColFunc::k_OnTriggerEnter);
                    }

                    //CHECKPOINT: NOTHING VALID, CHECK NEEDED
                    if(thisEnt)
                        thisEnt->DestroySafeRef(&thisEnt);
                    if(otherEnt)
                        otherEnt->DestroySafeRef(&otherEnt);
                }
            }
        }
        else//Update intruders.
        {
#if DEBUG_DRAW_COLLIDERS
            //Set the STATIC COLOR
            if (m_Colliders[thisI]->IsTrigger())
                /*DEBUG*/m_colliderObjects[thisI]->m_color = Color(Color::PreDefined::k_Blue);
            else if (m_colliderObjects[thisI]->m_color == Color(255,255,255,255))
                /*DEBUG*/m_colliderObjects[thisI]->m_color = Color(Color::PreDefined::k_Yellow);
            else
                /*DEBUG*/m_colliderObjects[thisI]->m_color = Color(Color::PreDefined::k_Red);
#endif
            /*Primary objective:
            * Here we want to use the intruders to check for TriggerStay with STATIONARY objects
            * and TriggerExit with MOVING objects.
            * Above we already took care of OnEnter
            * OnExit with STATIONARY object is dealt with autimatically (Collider comp destructor)
            */

            //CHECKPOINT: THIS_Col - VALID
            if (m_Colliders[thisI]->GetOverlaps() != nullptr /*m_Colliders[thisI]->GetOverlaps() > 0*/)
            {
                std::vector<Entity*>* overlaps = m_Colliders[thisI]->GetOverlaps();
        
                for (size_t overlapI = 0; overlapI < overlaps->size() && m_Colliders[thisI]; ++overlapI)
                {
                    //CHECKPOINT: THIS_Col - VALID
                    if ((*overlaps)[overlapI] != nullptr)
                    {
                        //CHECKPOINT: THIS_Col - VALID ****** OVERLAP - VALID

                        Entity* overlapEnt = (*overlaps)[overlapI]->GetSafeRef(&overlapEnt);
                        Entity* thisEnt = m_Colliders[thisI]->GetParent()->GetSafeRef(&thisEnt);
                        auto overlapCol = overlapEnt->GetComponent<EC_Collider>();
        
                        Rect thisRect = m_Colliders[thisI]->GetColWorldRect();
                        Rect overlapRect = overlapCol->GetColWorldRect();
        
        
                        if (AABBCheck(thisRect, overlapRect))
                        {
#if DEBUG_DRAW_COLLIDERS
                            //Set the ON COLLISION COLOR
                            if (m_Colliders[thisI]->IsTrigger())
                                /*DEBUG*/m_colliderObjects[thisI]->m_color = Color(Color::PreDefined::k_Green);
                            else
                                /*DEBUG*/m_colliderObjects[thisI]->m_color = Color(Color::PreDefined::k_Yellow);
#endif
                            CallColFunc(thisEnt, overlapEnt, overlapCol->GetDirection(), ColliderManager::ColFunc::k_OnTriggerStay);

                            //CHECKPOINT: NOTHING VALID, CHECK NEEDED.
                            if(overlapEnt && thisEnt)
                                CallColFunc(overlapEnt, thisEnt, m_Colliders[thisI]->GetDirection(), ColliderManager::ColFunc::k_OnTriggerStay);

                        }
                        else
                        {
                            m_Colliders[thisI]->RemoveOverlap(overlapEnt);
                            overlapCol->RemoveOverlap(thisEnt);
                            overlapI--;

                            CallColFunc(thisEnt, overlapEnt, overlapCol->GetDirection(), ColliderManager::ColFunc::k_OnTriggerExit);


                            //CHECKPOINT: NOTHING VALID, CHECK NEEDED.
                            if (overlapEnt && thisEnt)
                                CallColFunc(overlapEnt, thisEnt, m_Colliders[thisI]->GetDirection(), ColliderManager::ColFunc::k_OnTriggerExit);
                        }
                        //CHECKPOINT: NOTHING VALID, CHECK NEEDED.
                        if(thisEnt)
                            thisEnt->DestroySafeRef(&thisEnt);
                        if(overlapEnt)
                            overlapEnt->DestroySafeRef(&overlapEnt);
                    }
                }
            }
        }
    }

    //We run this separately because we are using its data during the loop above and changing
    //it would mess things up. We can also us this to solve other tasks.
    for (size_t i = 0; i < m_Colliders.size(); ++i)
    {
        if (m_Colliders[i] == nullptr)
        {
            m_Colliders.erase(m_Colliders.begin() + i);
#if DEBUG_DRAW_COLLIDERS
            /*DEBUG*/m_colliderObjects.erase(m_colliderObjects.begin() + i);
#endif
            continue;
        }

        m_Colliders[i]->ResetMoveState();
    }
}

std::vector<RenderableObject*>* ColliderManager::GetRenderableObjects()
{
    return &m_colliderObjects;
}

size_t ColliderManager::GetRenderableObjectsSize()
{
    return m_colliderObjects.size();
}

bool ColliderManager::AABBCheck(const Rect& thisCol, const Rect& otherCol)
{
    return (thisCol.x < otherCol.x + otherCol.w &&
        thisCol.x + thisCol.w > otherCol.x &&
        thisCol.y < otherCol.y + otherCol.h &&
        thisCol.y + thisCol.h > otherCol.y);
}

bool ColliderManager::HorizontalCheck(const Rect& thisCol, const Rect& otherCol)
{
    return (thisCol.x < otherCol.x + otherCol.w &&
        thisCol.x + thisCol.w > otherCol.x);
}

bool ColliderManager::VerticalCheck(const Rect& thisCol, const Rect& otherCol)
{
    return(thisCol.y < otherCol.y + otherCol.h &&
        thisCol.y + thisCol.h > otherCol.y);
}


/*
void ColliderManager::CallTriggerEnter(Entity* thisEntity, Entity* otherEntity, Point otherDir)
{
    std::vector<Component*>* comps = thisEntity->GetComponents();
    size_t compsSize = comps->size();
    Entity* thisEnt = thisEntity->GetSafeRef(thisEnt);
    Entity* otherEnt = otherEntity->GetSafeRef(otherEnt);


    for (size_t i = 0; i < compsSize; ++i)
    {
        if (thisEnt == nullptr || otherEnt == nullptr) return;

        (*comps)[i]->OnTriggerEnter(otherEnt, otherDir);
    }
    if(thisEnt)
        thisEnt->DestroySafeRef(thisEnt);
    if(otherEnt)
        otherEnt->DestroySafeRef(otherEnt);
}

void ColliderManager::CallTriggerStay(Entity* thisEntity, Entity* otherEntity, Point otherDir)
{
    std::vector<Component*>* comps = thisEntity->GetComponents();
    size_t compsSize = comps->size();
    Entity* thisEnt = thisEntity->GetSafeRef(thisEnt);
    Entity* otherEnt = otherEntity->GetSafeRef(otherEnt);


    for (size_t i = 0; i < compsSize; ++i)
    {
        if (thisEnt == nullptr || otherEnt == nullptr) return;

        (*comps)[i]->OnTriggerStay(otherEnt, otherDir);
    }
    if (thisEnt)
        thisEnt->DestroySafeRef(thisEnt);
    if (otherEnt)
        otherEnt->DestroySafeRef(otherEnt);
}

void ColliderManager::CallTriggerExit(Entity* thisEntity, Entity* otherEntity, Point otherDir)
{
    std::vector<Component*>* comps = thisEntity->GetComponents();
    size_t compsSize = comps->size();
    Entity* thisEnt = thisEntity->GetSafeRef(thisEnt);
    Entity* otherEnt = otherEntity->GetSafeRef(otherEnt);


    for (size_t i = 0; i < compsSize; ++i)
    {
        if (thisEnt == nullptr || otherEnt == nullptr) return;

        (*comps)[i]->OnTriggerExit(otherEnt, otherDir);
    }
    if (thisEnt)
        thisEnt->DestroySafeRef(thisEnt);
    if (otherEnt)
        otherEnt->DestroySafeRef(otherEnt);
}

void ColliderManager::CallOnCollision(Entity* thisEntity, Entity* otherEntity, Point otherDir)
{
    std::vector<Component*>* comps = thisEntity->GetComponents();
    size_t compsSize = comps->size();
    Entity* thisEnt = thisEntity->GetSafeRef(thisEnt);
    Entity* otherEnt = otherEntity->GetSafeRef(otherEnt);

    for (size_t i = 0; i < compsSize; ++i)
    {
        if (thisEnt == nullptr || otherEnt == nullptr) return;

        (*comps)[i]->OnCollision(otherEnt, otherDir);
    }
    if (thisEnt)
        thisEnt->DestroySafeRef(thisEnt);
    if (otherEnt)
        otherEnt->DestroySafeRef(otherEnt);
}
*/

void ColliderManager::CallColFunc(Entity* thisEntity, Entity* otherEntity, Point otherDir, ColFunc func)
{
    std::vector<Component*>* comps = thisEntity->GetComponents();
    size_t compsSize = comps->size();
    Entity* thisEnt = thisEntity->GetSafeRef(&thisEnt);
    Entity* otherEnt = otherEntity->GetSafeRef(&otherEnt);

    for (size_t i = 0; i < compsSize; ++i)
    {
        if (thisEnt == nullptr || otherEnt == nullptr) return;

        switch (func)
        {
        case ColliderManager::ColFunc::k_OnCollision:
            (*comps)[i]->OnCollision(otherEnt, otherDir);
            break;
        case ColliderManager::ColFunc::k_OnTriggerEnter:
            (*comps)[i]->OnTriggerEnter(otherEnt, otherDir);
            break;
        case ColliderManager::ColFunc::k_OnTriggerStay:
            (*comps)[i]->OnTriggerStay(otherEnt, otherDir);
            break;
        case ColliderManager::ColFunc::k_OnTriggerExit:
            (*comps)[i]->OnTriggerExit(otherEnt, otherDir);
            break;
        default:
            break;
        }
    }
    if (thisEnt)
        thisEnt->DestroySafeRef(&thisEnt);
    if (otherEnt)
        otherEnt->DestroySafeRef(&otherEnt);
}

//Returns true if the point p has collision, p is in world space.
bool ColliderManager::IsPointOverlapping(Point p, ColType colType)
{
    Rect point{ static_cast<int>(p.x), static_cast<int>(p.y), 1, 1 };
    Rect rect;

    for (EC_Collider* col : m_Colliders)
    {
        if (col == nullptr) continue;
        if (!col->IsActive()) continue;
        if (col->IsTrigger() && colType == ColType::k_Collision) continue;
        if (!col->IsTrigger() && colType == ColType::k_Trigger) continue;

        rect = col->GetColWorldRect();

        if (p.x >= rect.x && p.x <= rect.x + rect.w &&
            p.y >= rect.y && p.y <= rect.y + rect.h)
            return true;
    }

    return false;
}

bool ColliderManager::IsRectOverlapping(Rect rect, ColType colType)
{
    for (EC_Collider* col : m_Colliders)
    {
        if (col == nullptr) continue;
        if (!col->IsActive()) continue;
        if (col->IsTrigger() && colType == ColType::k_Collision) continue;
        if (!col->IsTrigger() && colType == ColType::k_Trigger) continue;

        if (AABBCheck(rect, col->GetColWorldRect()))
            return true;
    }

    return false;
}

//Is very important for this method that Entity::IsBeingDestroyed() works correctly.
//Is very important for this method that whenever an en is being destroyed, it 
//first sets being destroyed to true, then removes the collider from this manager and then
//deletes the collider component.
bool ColliderManager::IsPointOverlapping(Point p, std::vector<Entity*>* outOverlapEnts, ColType colType, bool checkColToo)
{
    Rect point{ static_cast<int>(p.x), static_cast<int>(p.y), 1, 1 };
    Rect rect;

    for (EC_Collider* col : m_Colliders)
    {
        if (col == nullptr) continue;
        if (!col->IsActive()) continue;
        if (col->IsTrigger() && colType == ColType::k_Collision) continue;
        if (!col->IsTrigger() && colType == ColType::k_Trigger) continue;

        rect = col->GetColWorldRect();

        if (p.x >= rect.x && p.x <= rect.x + rect.w &&
            p.y >= rect.y && p.y <= rect.y + rect.h)
        {
            Entity* colEnt = col->GetParent();
            if(!colEnt->IsBeingDestroyed())
                outOverlapEnts->push_back(colEnt);
        }
    }

    if (outOverlapEnts->size() > 0)
        return true;

    return false;
}

bool ColliderManager::IsRectOverlapping(Rect rect, std::vector<Entity*>* outOverlapEnts, ColType colType, bool checkColToo)
{
    for (EC_Collider* col : m_Colliders)
    {
        if (col == nullptr) continue;
        if (!col->IsActive()) continue;
        if (col->IsTrigger() && colType == ColType::k_Collision) continue;
        if (!col->IsTrigger() && colType == ColType::k_Trigger) continue;

        rect = col->GetColWorldRect();

        if (AABBCheck(rect, col->GetColWorldRect()))
        {
            Entity* colEnt = col->GetParent();
            if (!colEnt->IsBeingDestroyed())
                outOverlapEnts->push_back(colEnt);
        }
    }

    if (outOverlapEnts->size() > 0)
        return true;

    return false;
}