#include "UC_Demo7_Player.h"
#include "UC_Includes.h"
#include "UC_Shooter.h"
#include "UC_ShooterGameManager.h"
#include <iostream>

UC_ShooterGameManager* UC_Demo7_Player::m_pManager = nullptr;

UC_Demo7_Player::UC_Demo7_Player()
    : Component()
    , m_aimEnt(nullptr)
    , m_aimTransform(nullptr)
    , m_barrelTransform(nullptr)
    , m_barrelShape(nullptr)
    , m_aimSprite(nullptr)
    , m_shooter(nullptr)
    , m_hp(10)
    , m_bulletMinScalePer(5.0f)
    , m_bulletMaxScalePer(80.0f)
    , m_canAim(true)
    , m_canShoot(true)
    , m_canParry(true)
    , m_recoilState(0)
    , m_recoilDistance(70.0f)
    , m_recoilVelBack(800.0f)
    , m_recoilVelForward(700.0f)
    , m_barrelOriginPos(480, 480)
    , m_barrelEndPos(m_barrelOriginPos)
    , m_preRecoilBarrelEndPos(m_barrelEndPos)
    , m_preRecoilBarrelOriginPos(m_barrelOriginPos)
    , m_barrelLength(150.0f)//in pixels... we dont have a unit of measure... or we could say our unit of measure is pixels right now... so when we make an sprite, that sprite's dimensions in pixels is its default size.
    , m_barrelOriginShapeLength(100.0f)
    , m_barrelEndShapeLength(30.0f)
{}

UC_Demo7_Player::~UC_Demo7_Player()
{}

void UC_Demo7_Player::Init()
{
    //Check for dependency
    if (!m_pManager)
        Logger::Log(Logger::LOG_LEVEL::k_Error, "UC_ShooterManager is not accessible. You can't instantiate UC_Demo7_Player before UC_ShooterManager.");

    auto transform = m_pParent->AddComponent(new EC_Transform);
    auto col = m_pParent->AddComponent(new EC_Collider);

    transform->SetPos(256, 512);
    Rect colRect{-50,-50,100,100};
    col->SetColRect(colRect);
    col->SetTrigger(true);

    Rect aimSpriteRect{0,0,256,256};

    m_aimEnt = m_entityManager->AddEntity("PLAYER_AIM_ENT");
    m_aimTransform = m_aimEnt->AddComponent(new EC_Transform);
    m_aimSprite = m_aimEnt->AddComponent(new EC_Sprite);

    m_aimTransform->SetScale(0.25f, 0.25f);
    m_aimSprite->SetSourceImg("SS_CURSOR_Y.png", aimSpriteRect);
    m_aimSprite->SetPivotToCenter();
    m_aimSprite->SetLayer(1000);

    m_shooter = m_pParent->AddComponent(new UC_Shooter);

    m_shooter->SetProjectile(1,"Shooter/SP_DummyBall.png", Rect(0,0,128,128),20,0,1, m_bulletMinScalePer, m_bulletMaxScalePer);

    if (m_pManager)
    {
        m_shooter->AddTarget(m_pManager->GetEnemyOrbName());
        m_shooter->AddTarget(m_pManager->GetEnemyBossName());
        m_shooter->AddTarget("SHIELD_TRIGGER_ENT");
    }

    GameManager::Get()->ShowCursor(false);

    CreateBarrelPointer();
    CreateBarrelShape();
}

void UC_Demo7_Player::Update(float delta)
{
    if (!m_isActive) return;

    if (GameManager::Get()->GetDeltaVel() == 0.0f) return;

    UpdateAim(delta);
    UpdateShoot(delta);
    UpdateBarrelEndPos();
    UpdateBarrelShape();
    UpdateRecoil(delta);
}

//tells the aim transform to position itself where the pouse is.
void UC_Demo7_Player::UpdateAim(float delta)
{
    if (!m_canAim) return;

    m_aimTransform->SetPos(InputModule::Get()->GetMousePos());
}

//If shoot key pressed, we spawn a bullet at barrel's pos, aimed at where the aim transform
//(aka the mouse pos basically) is positioned.
void UC_Demo7_Player::UpdateShoot(float delta)
{
    if (!m_canShoot) return;

    //If we are aiming to a place where the barrel cannot get, then we just aim at where
    //the barrel is located.
    Point finalShootTargetPos;
    Point aimPos = m_aimTransform->GetPos();

    if(MathModule::Distance(aimPos, m_barrelOriginPos) < m_barrelLength)
        finalShootTargetPos = m_barrelEndPos + Point(0.5f, 0.5f);//This 0.5f point is a trick so that the Bullet has some distance to travel.
    else
        finalShootTargetPos = aimPos;

    if (InputModule::Get()->GetKeyState(KeyCode::k_LMB) == KeyState::k_KS_Release)
    {
        m_shooter->SpawnBullet(m_barrelEndPos, finalShootTargetPos);

        m_preRecoilBarrelOriginPos = m_barrelOriginPos;
        m_preRecoilBarrelEndPos = m_barrelEndPos;
        m_recoilState = 1;
        m_canShoot = false;
    }
}

void UC_Demo7_Player::CanAim(bool value)
{
    m_canAim = value;

    if (!m_canAim)
    {
        m_aimSprite->SetActive(false);
    }
}

void UC_Demo7_Player::CanShoot(bool value)
{
    m_canShoot = value;
    //careful we are not on recoil.
}

void UC_Demo7_Player::CanParry(bool value)
{
    m_canParry = value;
}

void UC_Demo7_Player::CreateBarrelPointer()
{
    auto ent = m_entityManager->AddEntity("BARREL_ENT");
    m_barrelTransform = ent->AddComponent(new EC_Transform);

    m_barrelTransform->SetScale(0.1f, 0.1f);
    m_barrelTransform->SetPos(256, 256);
}

void UC_Demo7_Player::UpdateBarrelEndPos()
{
    if (m_recoilState != 0) return;
    //get aim pos
    //get vector from barrel ORIGIN to aim pos
    //multiply for barrel length
    Point barrelDir = MathModule::GetDirection(m_barrelOriginPos, m_aimTransform->GetPos());
    m_barrelEndPos = m_barrelOriginPos + (barrelDir * m_barrelLength);
    m_barrelTransform->SetPos(m_barrelEndPos);
}

void UC_Demo7_Player::CreateBarrelShape()
{
    auto shapeEnt = m_entityManager->AddEntity("GUNSHAPE_H");
    auto shapeTransform = shapeEnt->AddComponent(new EC_Transform);
    m_barrelShape = shapeEnt->AddComponent(new EC_Shape);
    Quad2D q1{};
    m_barrelShape->AddQuad(q1);//qHorizontal
    m_barrelShape->AddQuad(q1);//qVertical
    m_barrelShape->AddQuad(q1);//qfrontal
    m_barrelShape->AddQuad(q1);//qhandler


    //Since we will be using the barrelOriginPos and barrelEndPos to determine the barrel
    //quads positions, we need to make sure the shape transform's is at pos 0, 0. because
    //the shape's quads positions is relative to its transform.
    shapeTransform->SetPos(0, 0);
}

void UC_Demo7_Player::UpdateBarrelShape()
{
    Quad2D qhorizontal;
    Quad2D qVertical;
    Quad2D qFrontal;
    Quad2D qHandler;

    //Update horizontal quad edge that corresponds with the origin of the barrel
    qhorizontal.p[0].x = m_barrelOriginPos.x - (m_barrelOriginShapeLength / 2.0f);
    qhorizontal.p[0].y = m_barrelOriginPos.y - (m_barrelOriginShapeLength / 2.0f);
    qhorizontal.p[3].x = m_barrelOriginPos.x + (m_barrelOriginShapeLength / 2.0f);
    qhorizontal.p[3].y = m_barrelOriginPos.y - (m_barrelOriginShapeLength / 2.0f);
    
    //Update horizontal quad edge that corresponds with the end of the barrel
    qhorizontal.p[1].x = m_barrelEndPos.x - (m_barrelEndShapeLength / 2.0f);
    qhorizontal.p[1].y = m_barrelEndPos.y - (m_barrelEndShapeLength / 2.0f);
    qhorizontal.p[2].x = m_barrelEndPos.x + (m_barrelEndShapeLength / 2.0f);
    qhorizontal.p[2].y = m_barrelEndPos.y - (m_barrelEndShapeLength / 2.0f);

    //Update vertical quad edge that corresponds with the origin of the barrel
    qVertical.p[0].x = m_barrelOriginPos.x - (m_barrelOriginShapeLength / 2.0f);
    qVertical.p[0].y = m_barrelOriginPos.y - (m_barrelOriginShapeLength / 2.0f);
    qVertical.p[1].x = m_barrelOriginPos.x - (m_barrelOriginShapeLength / 2.0f);
    qVertical.p[1].y = m_barrelOriginPos.y + (m_barrelOriginShapeLength / 2.0f);
    
    //Update vertical quad edge that corresponds with the end of the barrel
    qVertical.p[2].x = m_barrelEndPos.x - (m_barrelEndShapeLength / 2.0f);
    qVertical.p[2].y = m_barrelEndPos.y + (m_barrelEndShapeLength / 2.0f);
    qVertical.p[3].x = m_barrelEndPos.x - (m_barrelEndShapeLength / 2.0f);
    qVertical.p[3].y = m_barrelEndPos.y - (m_barrelEndShapeLength / 2.0f);

    //update frontal quad
    qFrontal.p[0] = qVertical.p[0];
    qFrontal.p[3] = qVertical.p[1];
    qFrontal.p[1] = qhorizontal.p[3];
    qFrontal.p[2].x = qFrontal.p[1].x;
    qFrontal.p[2].y = qFrontal.p[3].y;

    //update handler quad
    //float handlerDepthDist = 100.0f;
    //float handlerLength = 350.0f;
    float handlerDepthPerc = 45.0f;// in percentage. This defines the depth of the handler in percentage being 100% the origin to the tip of the cannon.
    float handlerLength = 350.0f;

    qHandler.p[0] = qVertical.p[1];
    qHandler.p[1].x = qHandler.p[0].x;
    qHandler.p[1].y = qHandler.p[0].y + handlerLength;

    float totalDist = MathModule::Distance(qHandler.p[0], qVertical.p[2]);
    Point handlerDir = MathModule::GetDirection(qHandler.p[0], qVertical.p[2]);

    handlerDepthPerc = totalDist / 100 * handlerDepthPerc;

    qHandler.p[3] = qVertical.p[1] + (handlerDir * handlerDepthPerc);
    qHandler.p[2].x = qHandler.p[3].x;
    qHandler.p[2].y = qHandler.p[3].y + handlerLength;


    m_barrelShape->EditQuad(0, qhorizontal);
    m_barrelShape->EditQuad(1, qVertical);
    m_barrelShape->EditQuad(2, qFrontal);
    m_barrelShape->EditQuad(3, qHandler);
}

void UC_Demo7_Player::UpdateRecoil(float delta)
{
    if (m_recoilState == 0) return;
    //note: make sure things check recoil state before proceeding.

    //set recoilState to 1 (1 means recoiling up, 2 means recoiling down, 0 means no recoil)
    //allow UpdateRecoil to go
    //  -if 1:

    //NOTE: START WITH JUST THE FIRST MOTION, AND ADD ON TOP AS YOU SUCCESSFULLY COMPLETE THEM
    if (m_recoilState == 1)
    {
        //update m_barrelEndPos
        Point recoilDir = MathModule::GetDirection(m_preRecoilBarrelEndPos, m_barrelOriginPos);
        m_barrelEndPos += recoilDir * m_recoilVelBack * delta;
        m_barrelOriginPos += recoilDir * m_recoilVelBack * delta;
        

        //if the recoil distane was achieved, update recoil state
        if (MathModule::Distance(m_barrelEndPos, m_preRecoilBarrelEndPos) >= m_recoilDistance)
        {
            m_barrelEndPos = m_preRecoilBarrelEndPos + (recoilDir * m_recoilDistance);

            m_barrelOriginPos = m_preRecoilBarrelOriginPos + (recoilDir * m_recoilDistance);
            m_recoilState = 2;
        }
    }
    else
    {
        Point recoilDir = MathModule::GetDirection(m_barrelOriginPos, m_preRecoilBarrelEndPos);
        m_barrelEndPos += recoilDir * m_recoilVelForward * delta;
        m_barrelOriginPos += recoilDir * m_recoilVelBack * delta;

        //if the recoil distane was achieved, update recoil state
        if (MathModule::Distance(m_barrelOriginPos, m_barrelEndPos) >= MathModule::Distance(m_barrelOriginPos, m_preRecoilBarrelEndPos))
        {
            m_barrelEndPos = m_preRecoilBarrelEndPos;
            m_barrelOriginPos = m_preRecoilBarrelOriginPos;
            m_recoilState = 0;
            m_canShoot = true;
        }
    }
}

void UC_Demo7_Player::OnHit(float dmg)
{
    m_hp--;
}