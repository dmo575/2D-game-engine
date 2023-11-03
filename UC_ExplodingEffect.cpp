#include "UC_ExplodingEffect.h"
#include "UC_Includes.h"

UC_ExplodingEffect::UC_ExplodingEffect(const char* spriteName, Rect spriteSrcRect, unsigned int anmLength, float anmSpeed, Point spawnPos, float scale)
    : m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_pAnimator(nullptr)
    , m_spriteName(spriteName)
    , m_spriteSrcRect(spriteSrcRect)
    , m_anmLength(anmLength)
    , m_anmSpeed(anmSpeed)
    , m_spawnPos(spawnPos)
    , m_scale(scale)
    , m_lifeSpam(m_anmSpeed* m_anmLength)
    , m_lifeSpamCounter(0.0f)
{}

UC_ExplodingEffect::~UC_ExplodingEffect()
{}

void UC_ExplodingEffect::Init()
{
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite);
    m_pAnimator = m_pParent->AddComponent(new EC_Animator);

    m_pTransform->SetPos(m_spawnPos);
    m_pTransform->SetScale(m_scale, m_scale);

    m_pSprite->SetSourceImg(m_spriteName.c_str(), m_spriteSrcRect);
    m_pSprite->SetPivotToCenter();

    m_pAnimator->SetSpeed(m_anmSpeed);
    m_pAnimator->AddClip("EXPLODE", m_anmLength, m_spriteSrcRect);
    m_pAnimator->SetClip("EXPLODE", true, false);
    m_pAnimator->Play(true);
}

void UC_ExplodingEffect::Update(float delta)
{
    if (GameManager::Get()->GetDeltaVel() == 0.0f) return;
    m_lifeSpamCounter += delta;

    if (m_lifeSpamCounter >= m_lifeSpam)
        m_pParent->Destroy();
}