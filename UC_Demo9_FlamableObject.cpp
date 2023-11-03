#include "UC_Demo9_FlamableObject.h"
#include "UC_Includes.h"
#include "UC_Demo9_FlameThrower.h"

unsigned int UC_Demo9_FlamableObject::m_instanceCounter = 0;
unsigned int UC_Demo9_FlamableObject::m_burntObjectCounter = 0;


UC_Demo9_FlamableObject::UC_Demo9_FlamableObject(UC_Demo9_Flamethrower* player, Point startPos)
    : m_pPlayer(player)
    , m_pSprite(nullptr)
    , m_pTransform(nullptr)
    , m_startPos(startPos)
    , m_minDistForBurn(150.0f)// min distance the flame thrower needs to be to start heating the object
    , m_heatNeededToMorph(.35f)//amnt of heat needed to morph the object
    , m_heatPoints(0.0f)//amnt of heat currently in the object
    , m_maxHeatInput(100.0f)//the maximun heat the object can receive per second
    , m_morphCounter(0)//keeps track of the morph level
{}

UC_Demo9_FlamableObject::~UC_Demo9_FlamableObject()
{}

void UC_Demo9_FlamableObject::Init()
{
    m_instanceCounter++;

    Rect srcRect{ 0,0,64,64 };
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite);

    m_pTransform->SetPos(m_startPos);
    m_pSprite->SetSourceImg("SP_HiddenBox.png", srcRect);
    m_pSprite->SetPivotToCenter();
}

void UC_Demo9_FlamableObject::Update(float delta)
{
    if (m_morphCounter == 10 || GameManager::Get()->GetDeltaVel() == 0.0f) return;


    Point flamePos = m_pPlayer->GetCurrentFlamePos();
    float flameDist = MathModule::Distance(flamePos, m_pTransform->GetPos());

    //If the flame is close enough to heat
    if (flameDist <= m_minDistForBurn && m_pPlayer->ThrowingFlames())
    {
        //store heat this frame
        m_heatPoints += (m_maxHeatInput / flameDist) * delta;

        //If heat stored is enough to morph the object
        if (m_heatPoints >= m_heatNeededToMorph)
        {
            //morph the object and reset the heat
            m_heatPoints = 0.0f;
            UpdateObjectVisual(+1);
        }
    }//This is ony checked when the flame is not reaching us. If we 
    else
    {
        //decrease heat points
        if(m_heatPoints > 0.0f)
            m_heatPoints -= m_maxHeatInput / 0.1f * delta;
        if (m_heatPoints < 0.0f)
            m_heatPoints = 0.0f;

        //if the heat points reached 0 and we are morphed
        if (m_heatPoints == 0.0f && m_morphCounter > 0)
        {
            //de-morph and set heat points to max.
            m_heatPoints = m_maxHeatInput;
            UpdateObjectVisual(-1);
        }
    }
}

void UC_Demo9_FlamableObject::UpdateObjectVisual(int mod)
{
    if (m_morphCounter == 10)
        return;
    else if (m_morphCounter + mod == 10)
    {
        m_burntObjectCounter++;
        if (m_burntObjectCounter == m_instanceCounter)
        {
            GameManager::Get()->ShowCursor(true);
            LevelManager::Get()->QueueLoadLevel("demo_9_lvl2");
        }
    }


    if (m_morphCounter + mod < 0)
        m_morphCounter = 0;
    else if (m_morphCounter + mod > 10)
        m_morphCounter = 10;
    else
        m_morphCounter += mod;

    Rect srcRect{static_cast<int>(m_morphCounter) * 64,0,64,64};
    m_pSprite->SetSourceRect(srcRect);
}