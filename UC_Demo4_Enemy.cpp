#include "UC_Demo4_Enemy.h"
#include "UC_Includes.h"
#include "UC_WASDController.h"
#include "UC_Demo4_HealthMod.h"

#include <string>

UC_Demo4_Enemy::UC_Demo4_Enemy(float vel, Point startPos, bool immortal)
    : m_pAnimator(nullptr)
    , m_pCollider(nullptr)
    , m_pSprite(nullptr)
    , m_pTransform(nullptr)
    , randomObj(Random::GetObject())
    , m_startPos(startPos)
    , m_countToDirChange(0.0f)
    , m_vel(vel)
    , m_currDir(0,1)
    , m_turnsCounter(0)
    , m_maxTurns(30)
    , m_timeCounter(0.0f)
    , m_maxTime(30.0f)
    , m_immortal(immortal)
    , Component()
{}

UC_Demo4_Enemy::~UC_Demo4_Enemy()
{}

void UC_Demo4_Enemy::Init()
{
    //Add components
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite(0U));
    m_pAnimator = m_pParent->AddComponent(new EC_Animator);
    m_pCollider = m_pParent->AddComponent(new EC_Collider);

    //Set up transform
    m_pTransform->SetPos(m_startPos);

    //Set up Sprite
    Rect rect64(0, 0, 64, 64);
    m_pSprite->SetSourceImg("SS_Skeleton_64x64.png", rect64);
    m_pSprite->SetPivotToCenter();

    //Set up Collider
    m_pCollider->AttachToSprite(false);
    m_pCollider->SetColRect(Rect(-12, -14, 24, 40));
    m_pCollider->SetTrigger(true);//We use them as triggers so we can overlap enemies crossing pahts

    //Set up animator
    m_pAnimator->SetSpeed(0.15f);
    m_pAnimator->AddClip("WALK_NORTH", 8, Rect{ 64,0,64,64 });
    m_pAnimator->AddClip("IDLE_NORTH", 1, Rect{ 0,0,64,64 });
    m_pAnimator->AddClip("WALK_SOUTH", 8, Rect{ 64,64 * 2,64,64 });
    m_pAnimator->AddClip("IDLE_SOUTH", 1, Rect{ 0,64 * 2,64,64 });
    m_pAnimator->AddClip("WALK_EAST", 8, Rect{ 64,64 * 3,64,64 });
    m_pAnimator->AddClip("IDLE_EAST", 1, Rect{ 0,64 * 3,64,64 });
    m_pAnimator->AddClip("WALK_WEST", 8, Rect{ 64,64,64,64 });
    m_pAnimator->AddClip("IDLE_WEST", 1, Rect{ 0,64,64,64 });

    m_pAnimator->Play(true);
    m_pAnimator->SetClip("IDLE_SOUTH");
}

void UC_Demo4_Enemy::Update(float delta)
{
    UpdateAnimationClip();
    UpdateMovement(delta);

    if (m_immortal) return;
        m_timeCounter+= delta;
    if (m_timeCounter >= m_maxTime)
        m_pParent->Destroy();
}

void UC_Demo4_Enemy::UpdateAnimationClip()
{
    if (m_currDir == Point(1, 0))
        m_pAnimator->SetClip("WALK_EAST");
    else if (m_currDir == Point(-1, 0))
        m_pAnimator->SetClip("WALK_WEST");
    else if (m_currDir == Point(0, 1))
        m_pAnimator->SetClip("WALK_SOUTH");
    else if (m_currDir == Point(0, -1))
        m_pAnimator->SetClip("WALK_NORTH");
}

void UC_Demo4_Enemy::OnTriggerStay(Entity* other, ColData data)
{
    if (other->IsBeingDestroyed()) return;
}

void UC_Demo4_Enemy::OnTriggerEnter(Entity* other, ColData data)
{
    if (other->GetName() == "Wall" || other->GetName() == "Door" || other->GetName() == "HealingArea")
    {
        GetNewDirection();
        UpdateTurns();
    }
}

void UC_Demo4_Enemy::GetNewDirection()
{

    if (m_immortal)
    {
        m_currDir.x *= -1;
        m_currDir.y *= -1;
        return;
    }


    Point tempDir(m_currDir.y, m_currDir.x);

    //The distance from the center of the collider to its border, opposite to the current direction.
    int colDist = tempDir.x != 0 ? m_pCollider->GetColWorldRect().w / 2 : m_pCollider->GetColWorldRect().h / 2;

    //Collider's rect in worlds space
    Rect colRect = m_pCollider->GetColWorldRect();

    //The center of the collider, in world space.
    Point colCenterWorld(colRect.x + colRect.w / 2,
        colRect.y + colRect.h / 2);


    /*Assigning a new direction, we check which direction the unit can use to move.
    * Rules:
    * -Can't use the same direction it currently has
    * -The opposite direction to its current direction its the last direction to consider
    * 
    * We first try checking one oposite side to the current direction
    * We then try the other opposite side
    * We finally assign the opposite direction to the current direction if we cant go any other direction.
    */

    //Point is always a point in world space to check for collisions, if no collisions are found in
    //point it means that the unit can move in point's direction.
    float randDir = randomObj->GetRandomValue(-1, 1) > 0 ? 1.0f : -1.0f;
    tempDir.x *= randDir;
    tempDir.y *= randDir;
    
    
    Point point = colCenterWorld;
    point.x += tempDir.x * (colDist + 2);
    point.y += tempDir.y * (colDist + 2);


    //Here we are checking if we can turn towards direction 1 (either left or right, but we check only one dir).
    if (!ColliderManager::Get()->IsPointOverlapping(point, ColliderManager::ColType::k_Collision))
    {
        m_currDir = tempDir;
        return;
    }

    tempDir.x *= -1;
    tempDir.y *= -1;

    point = colCenterWorld;
    point.x += tempDir.x * (colDist + 1);
    point.y += tempDir.y * (colDist + 1);

    //Here we check if we can turn the other direction we didn't check before.
    if (!ColliderManager::Get()->IsPointOverlapping(point, ColliderManager::ColType::k_Collision))
    {
        m_currDir = tempDir;
        return;
    }

    //If we reach this section it means we can't turn neither left nor right, so we choose going back.

    m_currDir.x *= -1;
    m_currDir.y *= -1;
}

void UC_Demo4_Enemy::UpdateMovement(float delta)
{
    if (m_currDir == Point(1, 0))
        m_pTransform->AddPos(m_vel * delta, 0.0f);
    else if (m_currDir == Point(-1, 0))
        m_pTransform->AddPos(-m_vel * delta, 0.0f);
    else if (m_currDir == Point(0, -1))
        m_pTransform->AddPos(0.0f, -m_vel * delta);
    else if (m_currDir == Point(0, 1))
        m_pTransform->AddPos(0.0f, m_vel * delta);
}

void UC_Demo4_Enemy::UpdateTurns()
{
    if (m_immortal) return;
        m_turnsCounter++;
    if (m_turnsCounter >= m_maxTurns)
        m_pParent->Destroy();
}