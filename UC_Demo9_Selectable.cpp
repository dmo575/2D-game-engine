#include "UC_Demo9_Selectable.h"
#include "UC_Includes.h"

UC_Demo9_Selectable::UC_Demo9_Selectable()
    : m_pTransform(nullptr)
    , m_pSpawner(nullptr)
    , m_pSprite(nullptr)
    , m_pSpawnerTransform(nullptr)
    , m_pInvertButton(nullptr)
    , m_invertValue(false)
    , m_selectionDistance(20.0f)
    , m_state(SelectableState::k_OffGrab)
{}

UC_Demo9_Selectable::~UC_Demo9_Selectable()
{}

void UC_Demo9_Selectable::Init()
{
    Rect srcRect{0,0,37,16};
    Rect btnRect{ 0,0,64,64 };
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite);

    m_pSprite->SetSourceImg("SP_Key.png", srcRect);
    m_pSprite->SetPivotToCenter();
    m_pSprite->SetLayer(1U);
    m_pTransform->SetPos(256, 256);

    auto spawnerEnt = m_entityManager->AddEntity("KeySpawner");
    m_pSpawner = spawnerEnt->AddComponent(new EC_ParticleSpawner(EC_ParticleSpawner::SpawnerType::k_Point));
    m_pSpawnerTransform = spawnerEnt->GetComponent<EC_Transform>();
    m_pSpawnerTransform->SetPos(256, 256);

    m_pSpawner->ParticleSetUp_Sprite("SP_Key.png", srcRect);
    m_pSpawner->SetParticleStartScale(0.5f);
    m_pSpawner->SetAccelerationMod(50.0f);
    m_pSpawner->SetScaleMod(.25f);
    m_pSpawner->SetDistToTravel(70.0f);
    m_pSpawner->SetVel(50.0f);
    m_pSpawner->SetSpawnRateTime(0.00f);//Spawn every frame
    m_pSpawner->SetSpawnRatePerSpawnCall(3, 5);//Spawn 3 to 5 keys every time you spawn keys
    m_pSpawner->SetActive(false);
    m_pSpawner->InvertLayerDepth(false);
    m_pSpawner->SetParticlesSpriteLayer(1U);

    auto btnEnt = m_entityManager->AddEntity("InvertButtonEnt");
    m_pInvertButton = btnEnt->AddComponent(new EC_Button);
    auto btnText = btnEnt->GetComponent<EC_Text>();
    m_pInvertButton->SetPressImg("SP_Button.png", btnRect);
    btnText->SetFont("Fonts/arial.ttf", 12);
    btnText->SetColor(Color::PreDefined::k_Black);
    btnText->SetText("  INVERT ! !");
    Point p{ 400, 256-32 };
    m_pInvertButton->SetPos(p);
    m_pInvertButton->Sub(Event::CLICKABLE_CLICK_UP, this);
}


void UC_Demo9_Selectable::Update(float delta)
{
    if (GameManager::Get()->GetDeltaVel() == 0.0f)
    {
        m_state = SelectableState::k_OffGrab;
        m_pSpawner->SetActive(false);
        return;
    }

    //if we click button and we are close, then set this guys as selected
    //while we are holding button and selected, set its pos to mouse pos
    //if we release clik, set this guy as not selected

    if (InputModule::Get()->GetKeyState(KeyCode::k_LMB) == KeyState::k_KS_Hold && m_state == SelectableState::k_OnGrab)
    {
        Point mousePos = InputModule::Get()->GetMousePos();
        m_pTransform->SetPos(mousePos);
        m_pSpawnerTransform->SetPos(mousePos);
    }
    else if (InputModule::Get()->GetKeyState(KeyCode::k_LMB) == KeyState::k_KS_Press)
    {
        Point mousePos = InputModule::Get()->GetMousePos();
        float dist = MathModule::Distance(mousePos, m_pSpawnerTransform->GetPos());
    
        if (dist <= m_selectionDistance)
        {
            m_state = SelectableState::k_OnGrab;
            m_pSpawner->SetActive(true);
        }
    }
    else if (InputModule::Get()->GetKeyState(KeyCode::k_LMB) == KeyState::k_KS_Release)
    {
        m_state = SelectableState::k_OffGrab;
        m_pSpawner->SetActive(false);
    }
}

void UC_Demo9_Selectable::Trigger(Event e, Entity* publisher)
{
    m_invertValue = !m_invertValue;
    m_pSpawner->InvertLayerDepth(m_invertValue);
}

UC_Demo9_Selectable::SelectableState UC_Demo9_Selectable::GetState()
{
    return m_state;
}