#pragma once
#include "Component.h"
#include "CoreStructs.h"
#include <vector>
#include <string>

//TODO: Maybe have a game manager for this and other things such as layers
//Althought layers.... I need to convert Z pos to layer number, use 2 digits
constexpr int k_fieldDistance = 8;//The distance that the bullet needs to travel in Z
constexpr float k_startEnemyBulletScale = 0.25f;
constexpr float k_startPlayerBulletScale = 1.0f;


//We assume player's Z value is 0
//We assume that player is looking at Z+ direction
//We assume that the enemy bullets spawn at Z k_filedMaxDist (8)
//We assume that player's bullets spawn at Z's 0
//We assume that regardless of X and Y pos, the bullets travel at the same speed in Z axis

/*How to controls the bullet's scale:   -   -   -   -   -   -   -   -   -   -   -   -   -   -
* We make our enemy bullet sprites the same dimension D1
* We make our player bullet sprites the same dimension D2
* We can control the minimum scale for enemy sprites by modifying k_startEnemyBulletScale
* We can control the mazimum scale for player sprites by modifying k_startPlayerBulletScale
* ... A better way would be to set a minimum and maximun scales, in percentage.
* Like, at max field distance, scale is 10%, at min field distance, scale is 100%, or 150%
* 
* 
* How to control the Z direction:   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
* Setting m_dir to >0 (+) means that the bullet will get further away each time
* Setting m_dir to <0 (-) means that the bullet will get closer each time
* 
* This means that player's dir is +
* This means that enemy's dir is -
* 
* Depending on the direction, bullet's Z value will either start at 0 or start at MAX
* Depending on the direction, bullet's Z value will either increase or decrease each update
* Bullets are set to be destroyed once Z's value goes out of range (field's range)
* 
* 
* //TODO:
* Set a standard for bullet's sprite dimensions:
* 64x64
* 
* 
* RIGHT NOW:
* -We update Z pos (we take into account where it starts and its direction)
* -We update 2D plan pos based on Z pos
* -We update scale based on Z pos and min/max scale values
* -We check if object has gone out of range in Z pos and act accordignly.
*/

class EC_Transform;
class EC_Sprite;
class EC_Collider;
class ColliderManager;
class Entity;
class UC_ShooterGameManager;


class UC_Bullet : public Component
{
    friend class UC_ShooterGameManager;
public:
    UC_Bullet(unsigned int t, Point startPos, Point targetPos, const char* img, Rect srcRect, float sp, float dmg, int travelDirZ, float minScale, float maxScale);
    virtual ~UC_Bullet() override;

    unsigned int GetType();
    void SetType(unsigned int newType);
    void AddTarget(const char* targetEntName);

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    void HitTargetIfAny();
    void SpawnDust();
    void SpawnPlayerSparks();

private:
    static UC_ShooterGameManager* m_pManager;
    static ColliderManager* m_pColManager;
    std::vector<std::string> m_targetNames;
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    Entity* m_dustEnt;
    unsigned int m_type;
    std::string m_imgFileName;
    Rect m_srcRect;
    Point m_dir;
    Point m_startPos;
    Point m_targetPos;
    float  m_speed;
    float m_dmg;
    float m_currPosZ;
    float m_2DDistance;//This is the distance that the bullet needs to travel in X,Y
    int m_travelDirZ;//+ or -. Used to know how to scale, low to high or high to low
    float m_zVelMult;
    float m_dustSpawnRate;
    float m_dustSpawnCounter;
    float m_minScalePer;
    float m_maxScalePer;
};