#pragma once
#include "Component.h"
#include "CoreStructs.h"
#include <iostream>
#include "Random.h"
#include <vector>
#include <string>

class EC_Transform;
class EC_Sprite;

class EC_ParticleSpawner : public Component
{
public:
    enum class SpawnerType { k_Point, k_Linear, k_PointToPointArea };
    enum class ParticleType { k_Sprite, k_Rect, k_Sprites, k_Rects };

private:
    class EC_ParticleSpark : public Component
    {
    private:
        friend class EC_ParticleSpawner;

    public:
        EC_ParticleSpark(std::string spriteSrcImg, Rect srcRect);//Spawns a sprite particle
        EC_ParticleSpark(Rect rectDimensions, Color rectColor);//Spawns a rect shaper particle
        virtual ~EC_ParticleSpark() override;

        void SetSpriteData(std::string spriteSrcImg, Rect srcRect);
        void SetRectData(Rect rectDimensions, Color rectColor);
        void InitParticle(Point startPos, float startScale, Point dir, float vel, float ttl, float scaleMod, float accelerationMod, unsigned int layer, bool invertDrawOrder);

    private:
        virtual void Init() override;
        virtual void Update(float delta) override;
        void DestroyParticle();


    private:
        EC_Transform* m_pTransform;
        EC_Sprite* m_pSprite;//TODO: Add a SetToBasicShape(true) and SetBasicShape(shape) into this and change the renderer.
        ParticleType m_type;
        Point m_dir;
        float m_vel;
        float m_ttl;//Time to live
        float m_ttlCounter;
        float m_scaleMod;
        float m_accelerationMod;

        std::string m_srcImg;//for sprite particles
        Color m_color;//for basic shape particles
        Rect m_srcRect;//for rect shape particles
    };

    class EC_ParticlePool : public Component
    {
    public:
        enum class PoolType { k_Manual, k_Auto };
    public:
        EC_ParticlePool();
        virtual ~EC_ParticlePool() override;

        size_t GetMaxCap();
        void SetMaxCap(size_t newCap);
        size_t GetPoolSize();
        bool HasSpace();
        bool HasParticles();
        float GetUpdateRate();
        void SetUpdateRate(float newUpdRate);
        PoolType GetPoolType();
        void SetPoolType(PoolType newType);
        EC_ParticleSpark* GetParticle();//this returns null if we have no particle
        bool AddParticle(EC_ParticleSpark* toAdd);

    private:
        virtual void Update(float delta) override;
        void TrimParticles(size_t newSize);
        bool AreSamplesOutdated();
        void UpdateSamples();

    private:
        std::vector<EC_ParticleSpark*> m_particles;
        size_t m_maxCap;
        PoolType m_type;
        float m_auto_updateRate;
        float m_auto_updateRateCounter;
        size_t m_auto_sampleAmnt;
        int* m_auto_currSamples;
        unsigned int m_auto_TrimPer;
        bool m_auto_canAddParticles;
    };

public:
    EC_ParticleSpawner(SpawnerType spawnerType);
    virtual ~EC_ParticleSpawner() override;

    //Calling any of these automatically make the spawner change the particle
    //type to spawn.
    void ParticleSetUp_Sprite(const char* img, Rect rect);
    void ParticleSetUp_Sprites(std::vector<std::string>* imgs, std::vector<Rect>* rects);
    void ParticleSetUp_Rect(Rect rectData, Color color);
    void ParticleSetUp_Rects(std::vector<Rect>* rects, std::vector<Color>* colors);


    //These are functions that change the values we pass to the particles, we can
    //either set a new value or increment/decrement the current value if we preffer
    //want a smooth transition.
    void SetParticleStartScale(float startScale);
    void SetAccelerationMod(float newVal);
    void ModAccelerationMod(float valToAdd);
    void SetScaleMod(float newValue);
    void ModScaleMod(float valueToAdd);
    void SetDistToTravel(float newValue);
    void ModDistToTravel(float valueToAdd);
    void SetVel(float newValue);
    void ModVel(float valueToAdd);
    void SetAppertureLinear(float newVal);
    void ModAppertureLinear(float valToAdd);
    void SetDirectionLinear(Point newVal);
    void SetSpawnRatePerSpawnCall(unsigned int min, unsigned int max);
    void SetSpawnRateTime(float newVal);
    void SetParticlesSpriteLayer(unsigned int layer);
    void InvertLayerDepth(bool value);

    //We use these to interface with the shared pool.
    static void UseAutomaticPooling(float poolUpdateRate = 10.0f)
    {
        m_sSharedPool->SetPoolType(EC_ParticlePool::PoolType::k_Auto);
    }
    static void UseManualPooling(size_t newMaxCapacity)
    {
        m_sSharedPool->SetPoolType(EC_ParticlePool::PoolType::k_Manual);
        m_sSharedPool->SetMaxCap(newMaxCapacity);
    }
    static size_t GetPoolSize()
    {
        return m_sSharedPool->GetMaxCap();
    }

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    void SpawnParticle();

private:
    static Entity* m_sSharedPoolEnt;
    static EC_ParticlePool* m_sSharedPool;
    static unsigned int m_sInstanceCounter;

    std::unique_ptr<Random::RandomObject> m_pRandom;
    SpawnerType m_type;
    ParticleType m_particleType;
    EC_Transform* m_transform;
    float m_particleVel;//Point and linear, velocity of each particle
    float m_particleDistToTravel;//Point and linear, distance the particle needs to travel from its spawn loc
    float m_particleSpawnRateSec;//Point and linear, spawn rate per second
    float m_timeCounter;
    float m_particleScaleMod;//Point and linear, scale mod to apply to each particle per delta time
    float m_particleAccelerationMod;
    std::vector<std::string> m_particleImgs;
    std::vector<Rect> m_particleSrcRects;
    std::vector<Color> m_particleColors;
    float m_particleStartScale;
    unsigned int m_minInstanceSpawnPerEvent;
    unsigned int m_maxInstanceSpawnPerEvent;
    unsigned int m_plarticlesSpriteLayer;

    unsigned int m_invertParticleDrawOrder;

    Point m_linear_dir;//Linear, direction to travel
    float m_linear_apperture;//Linear, offset to dest point, creates a cone-like shape of particles
};