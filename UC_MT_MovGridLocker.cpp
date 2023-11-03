#include "UC_MT_MovGridLocker.h"
#include "UC_Includes.h"
#include "UC_WASDController.h"
#include <math.h>

UC_MT_MovGridLocker::UC_MT_MovGridLocker(EC_Transform* t, UC_WASDController* c)
    : m_pTransform(t)
    , m_pController(c)
    , m_gridData(1.0f,1.0f)
    , m_startPos(0.0f, 0.0f)
    , m_currDir(0.0f, 0.0f)
    , m_currHostTile(0.0f, 0.0f)
    , m_moving(false)
{}

UC_MT_MovGridLocker::~UC_MT_MovGridLocker()
{}

void UC_MT_MovGridLocker::Init()
{
    if (m_pController)
        m_pController->ApplyMovement(false);
}

void UC_MT_MovGridLocker::Update(float delta)
{
    Point frameMovement(0.0f, 0.0f);
    Point controllerDir = m_pController->GetCurrDir();

    //Check for new movement.
    if(controllerDir != Point(0.0f, 0.0f) && !m_moving)
    {
        //Prepare to start a new moving
        m_moving = true;
        m_currDir = controllerDir;
        m_startPos = m_pTransform->GetPos();
        UpdateTargetPos(m_currDir);
    }

    if (!m_moving)
    {
        m_currDir.x = 0.0f;
        m_currDir.y = 0.0f;
        return;
    }

    //Calculate frame movement
    if(m_currDir.x != 0.0f)
        frameMovement.x = m_pController->GetVel() * delta;
    else if(m_currDir.y != 0.0f)
        frameMovement.y = m_pController->GetVel() * delta;

    //Apply movement
    m_pTransform->SetPos(m_pTransform->GetPos().x + (m_currDir.x * frameMovement.x), m_pTransform->GetPos().y + (m_currDir.y * frameMovement.y));

    //Check if we have reached our destination
    if (DestinationReached(frameMovement))
    {
        m_currHostTile = m_targetPos;
        m_pTransform->SetPos(m_targetPos);
        m_moving = false;
    }
}

void UC_MT_MovGridLocker::SetGridValue(Point grid)
{
    m_gridData = grid;
}

Point UC_MT_MovGridLocker::GetCurrDir()
{
    return m_currDir;
}

void UC_MT_MovGridLocker::UpdateTargetPos(Point& newDir)
{
    m_targetPos.x = m_pTransform->GetPos().x + (newDir.x * m_gridData.x);
    m_targetPos.y = m_pTransform->GetPos().y + (newDir.y * m_gridData.y);
}

bool UC_MT_MovGridLocker::DestinationReached(Point& frameMov)
{
    Point currPos = m_pTransform->GetPos();

    //Check if applying movement increases or reduces the distance between target and curr transform pos
    if (m_currDir.x != 0.0f)
    {
        float currDif = fabsf(fabsf(currPos.x) - fabsf(m_targetPos.x));
        float futureDif = fabsf((currPos.x + (m_currDir.x * frameMov.x)) - fabsf(m_targetPos.x));

        if (currDif > futureDif)
            return false;
    }
    else if (m_currDir.y != 0.0f)
    {
        float currDif = fabsf(fabsf(currPos.y) - fabsf(m_targetPos.y));
        float futureDif = fabsf((currPos.y + (m_currDir.y * frameMov.y)) - fabsf(m_targetPos.y));

        if (currDif > futureDif)
            return false;
    }

    return true;
}

void UC_MT_MovGridLocker::OnCollision(Entity* other, ColData data)
{
    //If we collide, we set our target position to be our original start pos and we set our
    //direction to be the opposite direction.
    m_currDir.x *= -1;
    m_currDir.y *= -1;
    m_targetPos = m_startPos;
}