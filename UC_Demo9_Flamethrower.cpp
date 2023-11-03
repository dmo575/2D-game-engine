#include "UC_Demo9_FlameThrower.h"
#include "UC_Includes.h"
#include "UC_Shooter.h"
#include "UC_ShooterGameManager.h"
#include <iostream>

UC_Demo9_Flamethrower::UC_Demo9_Flamethrower()
    : Component()
    , m_aimEnt(nullptr)
    , m_aimTransform(nullptr)
    , m_barrelTransform(nullptr)
    , m_barrelShape(nullptr)
    , m_aimSprite(nullptr)
    , m_pParticleSpawner(nullptr)
    , m_pSpawnerTransform(nullptr)
    , m_barrelOriginPos(480, 480)
    , m_barrelEndPos(m_barrelOriginPos)
    , m_preRecoilBarrelEndPos(m_barrelEndPos)
    , m_preRecoilBarrelOriginPos(m_barrelOriginPos)
    , m_barrelLength(150.0f)//in pixels... we dont have a unit of measure... or we could say our unit of measure is pixels right now... so when we make an sprite, that sprite's dimensions in pixels is its default size.
    , m_barrelOriginShapeLength(100.0f)
    , m_barrelEndShapeLength(30.0f)
    , m_flameConeMin(30.0f)
    , m_flameConeMax(70.0f)
{}

UC_Demo9_Flamethrower::~UC_Demo9_Flamethrower()
{}

void UC_Demo9_Flamethrower::Init()
{
    GameManager::Get()->ShowCursor(false);
    Rect aimSpriteRect{ 0,0,256,256 };

    auto particleSpawnerEnt = m_entityManager->AddEntity("Flamethrower_ParticleSp");
    m_pParticleSpawner = particleSpawnerEnt->AddComponent(new EC_ParticleSpawner(EC_ParticleSpawner::SpawnerType::k_Linear));
    m_pSpawnerTransform = particleSpawnerEnt->GetComponent<EC_Transform>();
    
    std::vector<std::string> imgs{8, "SP_Flames2.png"};
    std::vector<Rect> rects;
    rects.push_back({ 0,0,128,128 });
    rects.push_back({ 128,0,128,128 });
    rects.push_back({ 128*2,0,128,128 });
    rects.push_back({ 128*3,0,128,128 });
    rects.push_back({ 128*4,0,128,128 });
    rects.push_back({ 128*5,0,128,128 });
    rects.push_back({ 128*6,0,128,128 });
    rects.push_back({ 128*7,0,128,128 });


    m_pParticleSpawner->ParticleSetUp_Sprites(&imgs, &rects);
    m_pParticleSpawner->SetVel(800.0f);
    m_pParticleSpawner->SetSpawnRatePerSpawnCall(3, 5);
    
    auto transform = m_pParent->AddComponent(new EC_Transform);
    transform->SetPos(256, 512);

    m_aimEnt = m_entityManager->AddEntity("PLAYER_AIM_ENT");
    m_aimTransform = m_aimEnt->AddComponent(new EC_Transform);
    m_aimSprite = m_aimEnt->AddComponent(new EC_Sprite);

    m_aimTransform->SetScale(0.25f, 0.25f);
    m_aimSprite->SetSourceImg("SS_CURSOR_Y.png", aimSpriteRect);
    m_aimSprite->SetPivotToCenter();
    m_aimSprite->SetLayer(1000);

    CreateBarrelPointer();
    CreateBarrelShape();
}

void UC_Demo9_Flamethrower::Update(float delta)
{
    if (!m_isActive) return;
    if (GameManager::Get()->GetDeltaVel() == 0.0f) return;

    UpdateInput();
    UpdateAim(delta);
    UpdateBarrelEndPos();
    UpdateBarrelShape();
    UpdateParticleSpawner();
}

void UC_Demo9_Flamethrower::UpdateInput()
{
    if (InputModule::Get()->GetKeyState(KeyCode::k_LMB) == KeyState::k_KS_Hold)
        m_pParticleSpawner->SetActive(true);
    else
        m_pParticleSpawner->SetActive(false);
}

//tells the aim transform to position itself where the pouse is.
void UC_Demo9_Flamethrower::UpdateAim(float delta)
{
    m_aimTransform->SetPos(InputModule::Get()->GetMousePos());
}

void UC_Demo9_Flamethrower::UpdateParticleSpawner()
{
    m_pSpawnerTransform->SetPos(m_barrelEndPos);

    //dist that the particles must travel:
    float distToTravel = MathModule::Distance(m_barrelEndPos, m_aimTransform->GetPos());
    //dir the particles must travel:
    Point dirToTravel = MathModule::GetDirection(m_barrelEndPos, m_aimTransform->GetPos());
    //apperture of the cone:
    float apperture = m_flameConeMin + distToTravel;
    if (apperture > m_flameConeMax)
        apperture = m_flameConeMax;
    //startScale

    float sizeMod = 2.0f;
    float ttl = distToTravel / 800.0f;//800 is the vel we are using
    float sizeIncrementMod = sizeMod / ttl;
    m_pParticleSpawner->SetScaleMod(sizeIncrementMod);
    //Update particle spawner:

    m_pParticleSpawner->SetDistToTravel(distToTravel);
    m_pParticleSpawner->SetDirectionLinear(dirToTravel);
    m_pParticleSpawner->SetAppertureLinear(apperture);
    m_pParticleSpawner->SetParticleStartScale(apperture / 100.0f);

}

void UC_Demo9_Flamethrower::CreateBarrelPointer()
{
    auto ent = m_entityManager->AddEntity("BARREL_ENT");
    m_barrelTransform = ent->AddComponent(new EC_Transform);

    m_barrelTransform->SetScale(0.1f, 0.1f);
    m_barrelTransform->SetPos(256, 256);

    Rect srcRect{ 0,0,256,256 };
}

void UC_Demo9_Flamethrower::UpdateBarrelEndPos()
{
    //get aim pos
    //get vector from barrel ORIGIN to aim pos
    //multiply for barrel length
    Point barrelDir = MathModule::GetDirection(m_barrelOriginPos, m_aimTransform->GetPos());
    m_barrelEndPos = m_barrelOriginPos + (barrelDir * m_barrelLength);
    m_barrelTransform->SetPos(m_barrelEndPos);
}

void UC_Demo9_Flamethrower::CreateBarrelShape()
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

void UC_Demo9_Flamethrower::UpdateBarrelShape()
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

Point UC_Demo9_Flamethrower::GetCurrentFlamePos()
{
    return m_aimTransform->GetPos();
}

bool UC_Demo9_Flamethrower::ThrowingFlames()
{
    return m_pParticleSpawner->IsActive();
}