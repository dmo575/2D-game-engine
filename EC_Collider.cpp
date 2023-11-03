#include "EC_Collider.h"
#include "EC_Includes.h"
#include "ColliderManager.h"
#include "EC_Transform.h"
#include "EC_Sprite.h"

ColliderManager* EC_Collider::m_pColManager = nullptr;

EC_Collider::EC_Collider()
    : Component()
    , m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_colData(Rect())
    , m_lastPos(0, 0)
    , m_currPos(0, 0)
    , m_isTrigger(false)
    , m_moveState(true)//Stats as true since this col could be on top of another one
    , m_useSpriteRect(true)
{}

EC_Collider::~EC_Collider()
{
    for (size_t i = 0; i < m_overlaps.size(); ++i)
    {
        if (!m_overlaps[i]) continue;
        if (m_overlaps[i]->IsBeingDestroyed()) continue;

        auto overlapCol = m_overlaps[i]->GetComponent<EC_Collider>();
        overlapCol->OnTriggerExit(m_pParent, ColData(GetDirection()));
        overlapCol->RemoveOverlap(m_pParent);
    }
    m_pColManager->DestroyCollider(this);
}

void EC_Collider::Init()
{
    m_pTransform = m_pParent->GetComponent<EC_Transform>();

    if (!m_pTransform)
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("EC_Collider missing EC_Transform in \"" + m_pParent->GetName() + "\"").c_str());

    m_pSprite = m_pParent->GetComponent<EC_Sprite>();
    m_pColManager->AddCollider(this);

    m_currPos = m_pTransform->GetPos();
    m_lastPos = m_currPos;
}

void EC_Collider::Update(float delta)
{
    //We can't use this loop for Collider update, we need a loop that runs
    //alongside Collision update. This one runs once per frame.
}

//Update the collider
void EC_Collider::UpdateCollider(float delta)
{
    if (m_pTransform->GetPos() != m_currPos)
    {
        m_lastPos = m_currPos;
        m_currPos = m_pTransform->GetPos();
        m_moveState = true;
    }

}

//Returns true if the collider moved since the last time ResetMoveState() was called.
bool EC_Collider::GetMoveState()
{
    return m_moveState;
}

//Sets moveState to FALSE
void EC_Collider::ResetMoveState()
{
    m_moveState = false;
}

//If passed TRUE: Sets the collider rect to attach to the sprite's final in-game rect.
//If passed FALSE: Sets the collider rect to NOT be attached to the sprite
void EC_Collider::AttachToSprite(bool value)
{
    if (value && m_pSprite == nullptr)
    {
        m_pSprite = m_pParent->GetComponent<EC_Sprite>();
        if (m_pSprite == nullptr)
            Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("No EC_Sprite found for EC_Collider to attach to in \"" + m_pParent->GetName() + "\"").c_str());
    }

    m_useSpriteRect = value;
}

//Set the col's rect.
//Its position is relative to Transform position.
void EC_Collider::SetColRect(Rect newColRect)
{
    m_colData = newColRect;
}

//Returns the col's rect in world space and dimensions
const Rect EC_Collider::GetColWorldRect()
{
    if (m_useSpriteRect && m_pSprite != nullptr)
        return m_pSprite->GetWorldRect();

    return Rect(static_cast<int>(m_pTransform->GetPos().x + m_colData.x),
        static_cast<int>(m_pTransform->GetPos().y + m_colData.y),
        static_cast<int>(m_colData.w),
        static_cast<int>(m_colData.h));
}

//Returns true if the overlap is indeed a new overlap
bool EC_Collider::AddOverlap(Entity* overlap)
{
    for (size_t i = 0; i < m_overlaps.size(); ++i)
    {
        if (m_overlaps[i] == overlap)
            return false;
    }

    m_overlaps.push_back(overlap);
    return true;
}

//Removes the overlap from the vector
void EC_Collider::RemoveOverlap(Entity* overlap)
{
    for (size_t i = 0; i < m_overlaps.size(); ++i)
    {
        if (m_overlaps[i] == overlap)
        {
            m_overlaps.erase(m_overlaps.begin() + i);
            return;
        }
    }
}

//Returns a pointer to the overlaps vector
std::vector<Entity*>* EC_Collider::GetOverlaps()
{
    return &m_overlaps;
}

//value decides if the collider should behave like a trigger box
void EC_Collider::SetTrigger(bool value)
{
    m_isTrigger = value;
}

//Returns whether the collider is behaving as a trigger box or not
bool EC_Collider::IsTrigger()
{
    return m_isTrigger;
}

//Returns the direction the collider is moving at.
Point EC_Collider::GetDirection()
{
    return MathModule::GetDirection(m_lastPos, m_currPos);
}