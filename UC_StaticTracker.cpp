#include "UC_Includes.h"
#include "UC_StaticTracker.h"
#include <math.h>//         Need it for absolute number

UC_StaticTracker::UC_StaticTracker(EC_Transform* toTrack)
    : Component()
    , m_pTarget(toTrack)
    , m_pSprite(nullptr)
    , m_pTransform(nullptr)
    , m_range(50.0f)
{}

UC_StaticTracker::~UC_StaticTracker()
{}

void UC_StaticTracker::Init()
{
    m_pTransform = m_pParent->GetComponent<EC_Transform>();
    m_pSprite = m_pParent->GetComponent<EC_Sprite>();

    Rect spriteRect{0,0,70,117};
    m_pSprite->SetSourceImg("SP_Soldier_N.png", spriteRect);
}

void UC_StaticTracker::Update(float delta)
{
    Point me = m_pTransform->GetPos();
    Point target = m_pTarget->GetPos();
    Rect spriteRect{ 0,0,70,117 };

    if (GetDif(me.x, target.x) < m_range)
    {
        if (me.y < target.y)
            m_pSprite->SetSourceImg("SP_Soldier_S.png", spriteRect);
        else
            m_pSprite->SetSourceImg("SP_Soldier_N.png", spriteRect);
    }
    else if(GetDif(me.y, target.y) < m_range)
    {
        if (me.x < target.x)
            m_pSprite->SetSourceImg("SP_Soldier_E.png", spriteRect);
        else
            m_pSprite->SetSourceImg("SP_Soldier_W.png", spriteRect);
    }
    else
    {
        if (me.y < target.y)//Target below
        {
            if (me.x > target.x)
                m_pSprite->SetSourceImg("SP_Soldier_SW.png", spriteRect);
            else
                m_pSprite->SetSourceImg("SP_Soldier_SE.png", spriteRect);
        }
        else
        {
            if (me.x > target.x)
                m_pSprite->SetSourceImg("SP_Soldier_NW.png", spriteRect);
            else
                m_pSprite->SetSourceImg("SP_Soldier_NE.png", spriteRect);
        }
    }
}

float UC_StaticTracker::GetDif(float a, float b)
{
    float first, second;

    first = a > b ? a : b;
    second = a > b ? b : a;

    return (first - second);
}

void UC_StaticTracker::SetTarget(EC_Transform* newTarget)
{
    m_pTarget = newTarget;
}

void UC_StaticTracker::SetRange(float newRange)
{
    m_range = newRange;
}