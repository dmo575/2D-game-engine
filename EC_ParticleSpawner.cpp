#include "EC_ParticleSpawner.h"
#include "EC_Includes.h"
#include "EC_Transform.h"
#include "EC_Sprite.h"
#include "SpriteManager.h"
#include <assert.h>

EC_ParticleSpawner::EC_ParticlePool* EC_ParticleSpawner::m_sSharedPool = nullptr;
Entity* EC_ParticleSpawner::m_sSharedPoolEnt = nullptr;

unsigned int EC_ParticleSpawner::m_sInstanceCounter = 0;

EC_ParticleSpawner::EC_ParticleSpawner(SpawnerType spawnerType)
    : m_type(spawnerType)
    , m_particleType(ParticleType::k_Sprite)
    , m_transform(nullptr)
    , m_particleVel(500.0f)
    , m_particleDistToTravel(128.0f)
    , m_particleSpawnRateSec(0.05f)
    , m_particleScaleMod(.5f)
    , m_particleStartScale(1.0f)
    , m_particleAccelerationMod(0.0f)
    , m_linear_dir()
    , m_linear_apperture(0.0f)
    , m_timeCounter(0.0f)
    , m_minInstanceSpawnPerEvent(1)
    , m_maxInstanceSpawnPerEvent(1)
    , m_plarticlesSpriteLayer(0U)
    , m_invertParticleDrawOrder(true)
{
    m_sInstanceCounter++;
}

EC_ParticleSpawner::~EC_ParticleSpawner()
{
    m_sInstanceCounter--;

    //This will let us destroy the pool entity if there are no spawners in the level
    if (m_sInstanceCounter == 0 && m_sSharedPoolEnt)
    {
        m_sSharedPoolEnt->Destroy();
        m_sSharedPoolEnt = nullptr;
    }
}

void EC_ParticleSpawner::Init()
{
    m_pRandom = Random::GetObject();
    m_transform = m_pParent->AddComponent(new EC_Transform);

    //Init pool
    if (!m_sSharedPoolEnt)
    {
        m_sSharedPoolEnt = m_entityManager->AddEntity("_Shared_Pool");
        m_sSharedPool = m_sSharedPoolEnt->AddComponent(new EC_ParticlePool);
    }
}

void EC_ParticleSpawner::Update(float delta)
{
    m_timeCounter += delta;

    if (m_timeCounter >= m_particleSpawnRateSec)
    {
        unsigned int particlesToSpawn = m_pRandom->GetRandomValue(static_cast<unsigned int>(1), m_maxInstanceSpawnPerEvent);

        for (unsigned int i = 0; i < particlesToSpawn; ++i)
            SpawnParticle();

        m_timeCounter -= m_particleSpawnRateSec;
    }
}

void EC_ParticleSpawner::SpawnParticle()
{
    Entity* particleEnt = nullptr;
    EC_ParticleSpark* particleComp = nullptr;

    switch (m_particleType)
    {
    case EC_ParticleSpawner::ParticleType::k_Sprite:
    {
        if (m_sSharedPool->HasParticles())
        {
            particleComp = m_sSharedPool->GetParticle();
            particleComp->SetSpriteData(m_particleImgs[0], m_particleSrcRects[0]);
        }
        else
        {
            particleEnt = m_entityManager->AddEntity("Particle");
            particleComp = particleEnt->AddComponent(new EC_ParticleSpark(m_particleImgs[0], m_particleSrcRects[0]));
        }
        break;
    }
    case EC_ParticleSpawner::ParticleType::k_Rect:
    {
        if (m_sSharedPool->HasParticles())
        {
            particleComp = m_sSharedPool->GetParticle();
            particleComp->SetRectData(m_particleSrcRects[0], m_particleColors[0]);
        }
        else
        {
            particleEnt = m_entityManager->AddEntity("Particle");
            particleComp = particleEnt->AddComponent(new EC_ParticleSpark(m_particleSrcRects[0], m_particleColors[0]));
        }
        break;
    }
    case EC_ParticleSpawner::ParticleType::k_Sprites:
    {
        size_t elementIndex = static_cast<size_t>(m_pRandom->GetRandomValue(0, static_cast<int>(m_particleImgs.size() - 1)));
        
        if (m_sSharedPool->HasParticles())
        {
            particleComp = m_sSharedPool->GetParticle();
            particleComp->SetSpriteData(m_particleImgs[elementIndex], m_particleSrcRects[elementIndex]);
        }
        else
        {
            particleEnt = m_entityManager->AddEntity("Particle");
            particleComp = particleEnt->AddComponent(new EC_ParticleSpark(m_particleImgs[elementIndex], m_particleSrcRects[elementIndex]));
        }
        break;
    }
    case EC_ParticleSpawner::ParticleType::k_Rects:
    {
        size_t elementIndex = static_cast<size_t>(m_pRandom->GetRandomValue(0, static_cast<int>(m_particleSrcRects.size() - 1)));
        
        if (m_sSharedPool->HasParticles())
        {
            particleComp = m_sSharedPool->GetParticle();
            particleComp->SetRectData(m_particleSrcRects[elementIndex], m_particleColors[elementIndex]);
        }
        else
        {
            particleEnt = m_entityManager->AddEntity("Particle");
            particleComp = particleEnt->AddComponent(new EC_ParticleSpark(m_particleSrcRects[elementIndex], m_particleColors[elementIndex]));
        }
        break;
    }
    }

    //Calculate direction
    Point dir;

    switch (m_type)
    {
    case EC_ParticleSpawner::SpawnerType::k_Point:
    {
        dir = m_pRandom->GetRandomDirection(Random::DirectionAxisNumber::k_Free);
        break;
    }
    case EC_ParticleSpawner::SpawnerType::k_Linear:
    {
        int binaryValue = m_pRandom->GetRandomValue(0, 1);
        float distToTravelHalved = m_pRandom->GetRandomValue(0.0f, m_linear_apperture / 2.0f);
        Point conePerpendicularDir{m_linear_dir.y, m_linear_dir.x};

        if (binaryValue == 0)//turn the X negative
            conePerpendicularDir.x *= -1;
        else//turn the Y negative
            conePerpendicularDir.y *= -1;

        Point targetPos = (m_transform->GetPos() + (m_linear_dir * m_particleDistToTravel)) + (conePerpendicularDir * distToTravelHalved);
        dir = MathModule::GetDirection(m_transform->GetPos(), targetPos);
        break;
    }
    }

    //TTL is the lifespam of the particle
    float ttl = m_particleDistToTravel / m_particleVel;

    //Set up particle
    particleComp->InitParticle(m_transform->GetPos(), m_particleStartScale, dir, m_particleVel, ttl, m_particleScaleMod, m_particleAccelerationMod, m_plarticlesSpriteLayer, m_invertParticleDrawOrder);
}

void EC_ParticleSpawner::ParticleSetUp_Sprite(const char* img, Rect rect)
{
    m_particleImgs.clear();
    m_particleSrcRects.clear();
    m_particleColors.clear();
    m_particleType = ParticleType::k_Sprite;

    m_particleImgs.push_back(img);
    m_particleSrcRects.push_back(rect);
}

//For each sprite in the vector we will use its corresponding Rect in the rects vector.
void EC_ParticleSpawner::ParticleSetUp_Sprites(std::vector<std::string>* imgs, std::vector<Rect>* rects)
{
    assert(imgs->size() == rects->size());

    m_particleImgs.clear();
    m_particleSrcRects.clear();
    m_particleColors.clear();
    m_particleType = ParticleType::k_Sprites;

    for (size_t i = 0; i < imgs->size(); ++i)
    {
        m_particleImgs.push_back((*imgs)[i]);
        m_particleSrcRects.push_back((*rects)[i]);
    }
}

void EC_ParticleSpawner::ParticleSetUp_Rect(Rect rectData, Color color)
{
    m_particleImgs.clear();
    m_particleSrcRects.clear();
    m_particleColors.clear();
    m_particleType = ParticleType::k_Rect;

    m_particleSrcRects.push_back(rectData);
    m_particleColors.push_back(color);
}

void EC_ParticleSpawner::ParticleSetUp_Rects(std::vector<Rect>* rects, std::vector<Color>* colors)
{
    assert(rects->size() == colors->size());

    m_particleImgs.clear();
    m_particleSrcRects.clear();
    m_particleColors.clear();
    m_particleType = ParticleType::k_Rects;

    for (size_t i = 0; i < rects->size(); ++i)
    {
        m_particleSrcRects.push_back((*rects)[i]);
        m_particleColors.push_back((*colors)[i]);
    }
}

void EC_ParticleSpawner::SetScaleMod(float newVal)
{
    m_particleScaleMod = newVal;
}

void EC_ParticleSpawner::ModScaleMod(float valToAdd)
{
    SetScaleMod(m_particleScaleMod + valToAdd);
}

void EC_ParticleSpawner::SetAccelerationMod(float newVal)
{
    m_particleAccelerationMod = newVal;
}

void EC_ParticleSpawner::ModAccelerationMod(float valToAdd)
{
    SetAccelerationMod(m_particleAccelerationMod + valToAdd);
}

void EC_ParticleSpawner::SetDistToTravel(float newVal)
{
    m_particleDistToTravel = newVal;
}

void EC_ParticleSpawner::ModDistToTravel(float valToAdd)
{
    SetDistToTravel(m_particleDistToTravel + valToAdd);
}

void EC_ParticleSpawner::SetVel(float newVal)
{
    m_particleVel = newVal;
}

void EC_ParticleSpawner::ModVel(float valToAdd)
{
    SetVel(m_particleVel + valToAdd);
}

void EC_ParticleSpawner::SetAppertureLinear(float newVal)
{
    m_linear_apperture = newVal;
}

void EC_ParticleSpawner::ModAppertureLinear(float valToAdd)
{
    SetAppertureLinear(m_linear_apperture + valToAdd);
}

void EC_ParticleSpawner::SetDirectionLinear(Point newVal)
{
    m_linear_dir = newVal;
}

void EC_ParticleSpawner::SetSpawnRatePerSpawnCall(unsigned int min, unsigned int max)
{
    m_minInstanceSpawnPerEvent = min;
    m_maxInstanceSpawnPerEvent = max;
}

void EC_ParticleSpawner::SetParticleStartScale(float startScale)
{
    m_particleStartScale = startScale;
}

void EC_ParticleSpawner::SetSpawnRateTime(float newVal)
{
    m_particleSpawnRateSec = newVal;
}

void EC_ParticleSpawner::SetParticlesSpriteLayer(unsigned int layer)
{
    m_plarticlesSpriteLayer = layer;
}

void EC_ParticleSpawner::InvertLayerDepth(bool value)
{
    m_invertParticleDrawOrder = value;
}


//----------------------------------------------------------------------------------
//PARTICLE_SPARK    PARTICLE_SPARK  PARTICLE_SPARK  PARTICLE_SPARK  PARTICLE_SPARK
//----------------------------------------------------------------------------------

EC_ParticleSpawner::EC_ParticleSpark::EC_ParticleSpark(std::string spriteSrcImg, Rect srcRect)
    : m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_type(ParticleType::k_Sprite)
    , m_dir()
    , m_vel(0.0f)
    , m_ttl(0.0f)
    , m_ttlCounter(0.0f)
    , m_scaleMod(0.0f)
    , m_srcImg(spriteSrcImg)
    , m_color(Color::PreDefined::k_White)
    , m_srcRect(srcRect)
    , m_accelerationMod(0.0f)
{
}

EC_ParticleSpawner::EC_ParticleSpark::EC_ParticleSpark(Rect rectDimensions, Color rectColor)
    : m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_type(ParticleType::k_Rect)
    , m_dir()
    , m_vel(0.0f)
    , m_ttl(0.0f)
    , m_ttlCounter(0.0f)
    , m_scaleMod(0.0f)
    , m_srcImg("")
    , m_color(rectColor)
    , m_srcRect(rectDimensions)
    , m_accelerationMod(0.0f)
{
}

EC_ParticleSpawner::EC_ParticleSpark::~EC_ParticleSpark()
{}

void EC_ParticleSpawner::EC_ParticleSpark::Init()
{}

void EC_ParticleSpawner::EC_ParticleSpark::SetSpriteData(std::string spriteSrcImg, Rect srcRect)
{
    m_srcImg = spriteSrcImg;
    m_srcRect = srcRect;
    m_type = ParticleType::k_Sprite;
}

void EC_ParticleSpawner::EC_ParticleSpark::SetRectData(Rect rectDimensions, Color rectColor)
{
    m_srcRect = rectDimensions;
    m_color = rectColor;
    m_type = ParticleType::k_Rect;
}

void EC_ParticleSpawner::EC_ParticleSpark::InitParticle(Point startPos, float startScale, Point dir, float vel, float ttl, float scaleMod, float accelerationMod, unsigned int layer, bool invertDrawOrder)
{
    m_isActive = true;

    if (!m_pTransform)
        m_pTransform = m_pParent->AddComponent(new EC_Transform);

    if (!m_pSprite)
        m_pSprite = m_pParent->AddComponent(new EC_Sprite);
    else
        m_pSprite->SetActive(true);


    switch (m_type)
    {
    case EC_ParticleSpawner::ParticleType::k_Sprite:
    {
        m_pSprite->SetSourceImg(m_srcImg.c_str(), m_srcRect);
        break;
    }
    case EC_ParticleSpawner::ParticleType::k_Rect:
    {
        m_pSprite->SetBasicShape_Rect(m_srcRect, m_color);
        break;
    }
    }

    m_pTransform->SetPos(startPos);
    m_pTransform->SetScale(startScale, startScale);
    m_dir = dir;
    m_vel = vel;
    m_ttl = ttl;
    m_scaleMod = scaleMod;
    m_accelerationMod = accelerationMod;
    m_pSprite->SetPivotToCenter();
    m_pSprite->SetLayer(layer, invertDrawOrder);
}

void EC_ParticleSpawner::EC_ParticleSpark::Update(float delta)
{
    if (!m_isActive) return;


    m_ttlCounter += delta;

    if (m_ttlCounter < m_ttl)
    {
        m_pTransform->AddPos(m_dir * m_vel * delta);
        m_pTransform->AddScale(m_scaleMod * delta, m_scaleMod * delta);
        m_vel += m_accelerationMod * delta;
    }
    else
        DestroyParticle();
}

void EC_ParticleSpawner::EC_ParticleSpark::DestroyParticle()
{
    if (EC_ParticleSpawner::m_sSharedPool)
    {
        if (EC_ParticleSpawner::m_sSharedPool->HasSpace())
        {
            EC_ParticleSpawner::m_sSharedPool->AddParticle(this);
            m_isActive = false;
            m_pSprite->SetActive(false);
            m_ttlCounter = 0.0f;
            return;
        }
    }

    m_pParent->Destroy();
}

//----------------------------------------------------------------------------------
//PARTICLE_POOL     PARTICLE_POOL   PARTICLE_POOL   PARTICLE_POOL   PARTICLE_POOL
//----------------------------------------------------------------------------------


EC_ParticleSpawner::EC_ParticlePool::EC_ParticlePool()
    : m_maxCap(0)
    , m_type(PoolType::k_Auto)
    , m_auto_updateRate(2.0f)
    , m_auto_updateRateCounter(0.0f)
    , m_auto_sampleAmnt(50)
    , m_auto_currSamples(new int[m_auto_sampleAmnt] {-1})
    , m_auto_TrimPer(10U)//percentage of vector's size to trim
    , m_auto_canAddParticles(true)
{}


EC_ParticleSpawner::EC_ParticlePool::~EC_ParticlePool()
{
    delete[] m_auto_currSamples;
    EC_ParticleSpawner::m_sSharedPoolEnt = nullptr;
    EC_ParticleSpawner::m_sSharedPool = nullptr;

    for (size_t i = 0; i < m_particles.size(); ++i)
        m_particles[i]->GetParent()->Destroy();

    m_particles.clear();
}

void EC_ParticleSpawner::EC_ParticlePool::Update(float delta)
{
    if (m_type == PoolType::k_Manual) return;

    if (m_particles.size() < m_auto_sampleAmnt)
    {
        m_auto_canAddParticles = true;
        m_auto_updateRateCounter = 0.0f;
        return;
    }

    m_auto_updateRateCounter += delta;

    if (m_auto_updateRateCounter >= m_auto_updateRate)
    {
        //too few particles, need more
        if (AreSamplesOutdated())
        {
            

            UpdateSamples();
            m_auto_canAddParticles = true;
        }//too many particles, need to release some
        else
        {
            //I casted everything to float here but it still throws that warning onto me, I can't tell what it means
            size_t newSize = static_cast<size_t>(static_cast<float>(static_cast<float>(m_particles.size()) / 100.0f) * static_cast<float>(100.0f - static_cast<float>(m_auto_TrimPer)));

            TrimParticles(newSize);
            m_auto_canAddParticles = false;
        }

        m_auto_updateRateCounter -= m_auto_updateRate;
    }
}

size_t EC_ParticleSpawner::EC_ParticlePool::GetMaxCap()
{
    return m_maxCap;
}

void EC_ParticleSpawner::EC_ParticlePool::SetMaxCap(size_t newCap)
{
    if (newCap > m_maxCap)
        TrimParticles(newCap);

    m_maxCap = newCap;
}

size_t EC_ParticleSpawner::EC_ParticlePool::GetPoolSize()
{
    return m_particles.size();
}

bool EC_ParticleSpawner::EC_ParticlePool::HasSpace()
{
    if(m_type == PoolType::k_Manual)
        return m_particles.size() < m_maxCap;
    else
        return m_auto_canAddParticles;
}

bool EC_ParticleSpawner::EC_ParticlePool::HasParticles()
{
    return m_particles.size() > 0;
}

float EC_ParticleSpawner::EC_ParticlePool::GetUpdateRate()
{
    return m_auto_updateRate;
}

void EC_ParticleSpawner::EC_ParticlePool::SetUpdateRate(float newRate)
{
    m_auto_updateRate = newRate;
}

EC_ParticleSpawner::EC_ParticlePool::PoolType EC_ParticleSpawner::EC_ParticlePool::GetPoolType()
{
    return m_type;
}

void EC_ParticleSpawner::EC_ParticlePool::SetPoolType(PoolType newType)
{
    m_type = newType;
}

EC_ParticleSpawner::EC_ParticleSpark* EC_ParticleSpawner::EC_ParticlePool::GetParticle()
{
    if (m_particles.size() == 0) return nullptr;

    auto p = m_particles[m_particles.size() - 1];
    m_particles.erase(m_particles.begin() + m_particles.size() - 1);
    return p;
}

bool EC_ParticleSpawner::EC_ParticlePool::AddParticle(EC_ParticleSpark* toAdd)
{
    if (HasSpace())
    {
        m_particles.push_back(toAdd);
        return true;
    }

    return false;
}

void EC_ParticleSpawner::EC_ParticlePool::TrimParticles(size_t newSize)
{
    if (newSize <= m_particles.size()) return;

    for (size_t i = newSize; i < m_particles.size(); ++i)
        m_particles[i]->GetParent()->Destroy();

    m_particles.erase(m_particles.begin() + newSize, m_particles.end());
}


//Updates the samples with the ID or -1 if there are not enough particles to cover all
//the spaces
void EC_ParticleSpawner::EC_ParticlePool::UpdateSamples()
{
    size_t loopAmnt = m_auto_sampleAmnt < m_particles.size() ? m_auto_sampleAmnt : m_particles.size();

    for (size_t i = 0; i < loopAmnt; ++i)
        m_auto_currSamples[i] = m_particles[i]->GetParent()->GetID();
    
    for (size_t i = loopAmnt; i < m_auto_sampleAmnt; ++i)
        m_auto_currSamples[i] = -1;

}

bool EC_ParticleSpawner::EC_ParticlePool::AreSamplesOutdated()
{
    //checks samples from last to first.
    for (size_t i = m_auto_sampleAmnt - 1; i > 0; --i)
    {
        if (m_auto_currSamples[i] != m_particles[i]->GetParent()->GetID())
            return true;
    }

    return false;
}