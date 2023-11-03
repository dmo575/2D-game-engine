#include "UC_Bulltet.h"
#include "UC_Includes.h"
#include "IShooterHittable.h"
#include "UC_Demo7_OrbEnemy.h"
#include "UC_Demo7_Player.h"
#include "UC_Dust.h"
#include "UC_ShieldTrigger.h"
#include "UC_Demo7_Boss.h"
#include "UC_ShooterGameManager.h"
#include <iostream>
#include <vector>

ColliderManager* UC_Bullet::m_pColManager = nullptr;
UC_ShooterGameManager* UC_Bullet::m_pManager = nullptr;

UC_Bullet::UC_Bullet(unsigned int t, Point startPos, Point targetPos, const char* img, Rect srcRect, float sp, float dmg, int travelDirZ, float minScale, float maxScale)
    : Component()
    , m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_dustEnt(nullptr)
    , m_type(t)
    , m_dir(0, 0)
    , m_startPos(startPos)
    , m_targetPos(targetPos)
    , m_imgFileName(img)
    , m_srcRect(srcRect)
    , m_speed(sp)
    , m_dmg(dmg)
    , m_currPosZ(0.0f)
    , m_2DDistance(0.0f)
    , m_travelDirZ(travelDirZ)
    , m_zVelMult(5.5f)//Lets first use this to reate the line and see if it works.
    , m_dustSpawnRate(0.025f)
    , m_dustSpawnCounter(0.0f)
    , m_minScalePer(minScale)
    , m_maxScalePer(maxScale)
{}

UC_Bullet::~UC_Bullet()
{}

void UC_Bullet::Init()
{
    if (!m_pColManager)
        m_pColManager = ColliderManager::Get();

    //Add components
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite);

    //Set up components
    if (m_travelDirZ != 1 && m_travelDirZ != -1)
        Logger::Log(Logger::LOG_LEVEL::k_Error, "UC_Bullet: Travel direction has to be either 1 or -1");

    m_currPosZ = m_travelDirZ == 1 ? 0.0f : k_fieldDistance;

    float scale = k_startEnemyBulletScale + m_currPosZ;
    m_pTransform->SetScale(scale, scale);
    m_pTransform->SetPos(m_startPos);

    m_pSprite->SetSourceImg(m_imgFileName.c_str(), m_srcRect);
    m_pSprite->SetPivotToCenter();


    m_2DDistance = MathModule::Distance(m_pTransform->GetPos(), m_targetPos);
    m_dir = MathModule::GetDirection(m_startPos, m_targetPos);
}

void UC_Bullet::Update(float delta)
{
    if (GameManager::Get()->GetDeltaVel() == 0.0f) return;

    //Update Z
    //Depending on the direction the bullet is traveling, we will either increase or decrease
    //Z's value
    m_currPosZ += m_travelDirZ == 1 ? (m_speed * delta) : -(m_speed * delta);

    float zDistanceFromStart = m_travelDirZ == 1 ? m_currPosZ : (k_fieldDistance - m_currPosZ);


    //The reason for the next lines is:
    //To help convey the idea of perspective and Z velocity.
    //Why: Because without this the bullets look like they travel slow at close view and
    //accelerate as they get further away, this is because in our X and Y, we move the bullet
    //at a constant speed but if we want to fake 3D we need to change that so that the 
    //"closer" the object is in the Z axis, the faster it travels.

    //Create a "1D line" of the size of the field times zVelMult
    float zMultLineDistance = k_fieldDistance * m_zVelMult;

    //This is:
    //-Treat the field distance as a 1D line as well
    //-Get the distance from 0 to our position in the Z line
    //-Get the proportionally equal distance from 0 to our zMultLineDistance -> we call it X
    //-Get the result of zMultLineDistance - X -> This is multFromLine
    float multFromLine = zMultLineDistance - ((zMultLineDistance / k_fieldDistance) * m_currPosZ);

    if (multFromLine < 0.0f)
        multFromLine = 0.0f;


    m_currPosZ = m_travelDirZ == 1 ? (m_currPosZ + multFromLine * delta) : (m_currPosZ - multFromLine * delta);

    //Add multFromLine times delta to our Z position.


    //Convert field distance traveled (m_currPosZ) to 2D distance traveled:

    //We now calculate how much of the Z's distance we traveled would amount to in
    //X,Y distance traveled:
    //We have this distance for the bullet to travel in the Z axis called field distance.
    //We also have to move the bullet id 2D (x,Y). But we want the bullets to take the same
    //time to make it to their destination regarding on where in the 2D plane they spawned.
    //So we use the current Z position we have and the distance it has to travel to calculate
    //the distance the bullet traveled in 2D. If the distance traveled in Z is 45% of its
    //total, then the distance we will move the bullet in the 2D plane will be 45% of its
    //total. This mixed with the scale of the bullet will help give the impression of
    //depth, hopefully...
    //float distanceTraveled2D = (m_2DDistance / k_fieldDistance) * m_currPosZ;
    float distanceTraveled2D = (m_2DDistance / k_fieldDistance) * zDistanceFromStart;

    //We now add a multiplier that changes depending on the Z position
    //The closer the z pos is to 0 (aka the closer the bullet is to the player, aka
    //the bigger the bullet is on screen), the higher this multiplayer will be.
    //We do this to help convey a consisten 3D speed to the bullet.
    //If the bullet's Z value is the higher it can be (field's value), then this mult
    // will be 0.0f.
    //If the bullet's Z value is 0, then this mult will be the maximum value it can
    //be (m_maxSpeedMult)

    m_pTransform->SetPos(m_startPos + (m_dir * distanceTraveled2D));

    //Update sprite's layer number based on Z position
    //We do this to ensure than bullets get drawn based in their Z pos
    //Each Z full unit equals 10 layer units
    //Each Z decimal unit equals 1 layer unit
    //This means a Z value of 15.233 equals to layer value 152
    int layer = (int)m_currPosZ * 10;
    int secondDigit = (int)(m_currPosZ - m_currPosZ) * 10;
    layer += secondDigit;
    m_pSprite->SetLayer(layer);


    //Set scale based on z pos.
    //Calculate bullet scale based on Z pos and min/max percentages
    float scaleRange = m_maxScalePer - m_minScalePer;
    float zScaleMod = (scaleRange / k_fieldDistance) * (k_fieldDistance - m_currPosZ);
    float finalScale = (m_minScalePer / 100.0f) + (zScaleMod / 100.0f);

    m_pTransform->SetScale(finalScale, finalScale);


    //DUST CODE
    m_dustSpawnCounter += delta;
    if (m_dustSpawnCounter >= m_dustSpawnRate)
    {
        SpawnDust();
        m_dustSpawnCounter -= m_dustSpawnRate;
    }

    //Check if bullet traveled past the field's range. If so, destroy it.
    if (m_currPosZ > k_fieldDistance || m_currPosZ < 0.0f)
    {
        HitTargetIfAny();
        m_pParent->Destroy();
    }
}

unsigned int UC_Bullet::GetType()
{
    return m_type;
}

void UC_Bullet::SetType(unsigned int newType)
{
    m_type = newType;
}

void UC_Bullet::HitTargetIfAny()
{
    std::vector<Entity*> targets;

    if (m_pColManager->IsPointOverlapping(m_pTransform->GetPos(), &targets))
    {
        //If target is not self and target is not bullet (id by name)
        if (targets[0] != m_pParent && targets[0]->GetName() != m_pParent->GetName())
        {
            for (auto name : m_targetNames)
            {
                if (name == targets[0]->GetName())
                {
                    IShooterHittable* hittable = nullptr;
                    if (name == m_pManager->GetEnemyOrbName())
                    {
                        SpawnPlayerSparks();
                        UC_Demo7_OrbEnemy* enemyOrbComp = targets[0]->GetComponent<UC_Demo7_OrbEnemy>();
                        hittable = enemyOrbComp;
                    }
                    else if (name == m_pManager->GetPlayerName())
                    {
                        UC_Demo7_Player* playerComp = targets[0]->GetComponent<UC_Demo7_Player>();
                        hittable = playerComp;
                    }
                    else if (name == m_pManager->GetEnemyBossName())
                    {
                        SpawnPlayerSparks();
                        UC_Demo7_Boss* playerComp = targets[0]->GetComponent<UC_Demo7_Boss>();
                        hittable = playerComp;
                    }
                    else if (name == "SHIELD_TRIGGER_ENT")
                    {
                        SpawnPlayerSparks();
                        UC_ShieldTrigger* playerComp = targets[0]->GetComponent<UC_ShieldTrigger>();
                        hittable = playerComp;
                    }


                    if(hittable)
                        hittable->OnHit(m_dmg);
                    break;
                }
            }
        }
    }
}

void UC_Bullet::SpawnPlayerSparks()
{
    //spawn various small dust ents going in all directions at fast speed
    Point directions[8];
    directions[0] = MathModule::GetDirection(Point{ 0,0 }, Point{ 0,-1 });
    directions[1] = MathModule::GetDirection(Point{ 0,0 }, Point{ 0,1 });
    directions[2] = MathModule::GetDirection(Point{ 0,0 }, Point{ 1,0 });
    directions[3] = MathModule::GetDirection(Point{ 0,0 }, Point{ -1,0 });
    directions[4] = MathModule::GetDirection(Point{ 0,0 }, Point{ 1,-1 });
    directions[5] = MathModule::GetDirection(Point{ 0,0 }, Point{ -1,-1 });
    directions[6] = MathModule::GetDirection(Point{ 0,0 }, Point{ 1,1 });
    directions[7] = MathModule::GetDirection(Point{ 0,0 }, Point{ 1,-1 });

    for (size_t i = 0; i < 8; ++i)
    {
        std::string spriteName = "Shooter/SP_DummyBall.png";
        Rect srcRect{ 32, 32, 10, 10 };
        float vel = 300.0f;
        float scaleMult = 1.1f;
        float lifeSpam = 0.08f;
        unsigned int layer = m_pSprite->GetLayer() + 1;
        auto dustEnt = m_entityManager->AddEntity("DUST_ENT");
        float scale = 1.0f;
        dustEnt->AddComponent(new UC_Dust(scale, 1, spriteName.c_str(), srcRect, m_pTransform->GetPos(), directions[i] * -1, vel, scaleMult, lifeSpam, layer));
    }
}

void UC_Bullet::AddTarget(const char* targetEntName)
{
    m_targetNames.push_back(targetEntName);
}

void UC_Bullet::SpawnDust()
{
    Rect srcRect{64, 64, 10, 10};
    Point travelDir{-m_dir.x, -m_dir.y};
    float vel = 5.0f;
    float scaleMult = 1.1f;//must always be more than 1.0f, focus on vel. Dust takes care of applying to scale up or down
    float lifeSpam = 0.15f;
    unsigned int layer = m_pSprite->GetLayer() - 50;//this should automatically be set to 0 if we go under it
    auto dustEnt = m_entityManager->AddEntity("DUST_ENT");
    dustEnt->AddComponent(new UC_Dust(m_pTransform->GetScale().x, 1, m_imgFileName.c_str(), srcRect, m_pTransform->GetPos(), travelDir * -1, vel, scaleMult, lifeSpam, layer));
}