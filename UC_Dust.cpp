#include "UC_Dust.h"
#include "UC_Includes.h"
#include <iostream>

UC_Dust::UC_Dust(float initialScale, int scaleDir, const char* sprite, Rect spriteSrcRect, Point spawnPos, Point travelDir, float vel, float scaleMult, float lifeSpam, int layer)
    : m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , randomObj(Random::GetObject())
    , m_srcRect(spriteSrcRect)
    , m_spawnPos(spawnPos)
    , m_travelDirOriginal(travelDir)
    , m_travelDirFinal()
    , m_spriteName(sprite)
    , m_initialScale(initialScale)
    , m_scaleDir(scaleDir)
    , m_dustVel(vel)
    , m_scaleMult(scaleMult)
    , m_lifeSpam(lifeSpam)
    , m_timeCounter(0.0f)
    , m_layer(layer)
{}

UC_Dust::~UC_Dust()
{}

void UC_Dust::Init()
{
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite);

    //set up transform
    m_pTransform->SetPos(m_spawnPos);
    m_pTransform->SetScale(m_initialScale, m_initialScale);
    
    //set up sptire:
    m_pSprite->SetSourceImg(m_spriteName.c_str(), m_srcRect);
    m_pSprite->SetLayer(m_layer);

    //calculate final travel direction
    Point originPoint = m_travelDirOriginal;
    Point secondPoint = m_travelDirOriginal * 10.0f;//The larget this number, the shorter the final diff
    secondPoint += randomObj->GetRandomPoint(0.0f, 2.5f);//the larger the max number, the larger the final diff


    m_travelDirFinal = MathModule::GetDirection(originPoint, secondPoint);
}

void UC_Dust::Update(float delta)
{
    if (GameManager::Get()->GetDeltaVel() == 0.0f) return;
    UpdateDustPos(delta);
    UpdateLifeSpam(delta);
}

void UC_Dust::UpdateDustPos(float delta)
{
    m_pTransform->AddPos(m_travelDirFinal * delta* m_dustVel);
    m_pTransform->AddScale(m_scaleMult * delta * m_scaleDir, m_scaleMult * delta * m_scaleDir);
}

void UC_Dust::UpdateLifeSpam(float delta)
{
    m_timeCounter += delta;

    if (m_timeCounter >= m_lifeSpam)
        m_pParent->Destroy();
}