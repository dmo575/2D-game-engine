#include "UC_Cursor.h"
#include "UC_Includes.h"

UC_Cursor::UC_Cursor(float vel)
    : Component()
    , m_pTransform(nullptr)
    , m_pAnimator(nullptr)
    , m_vel(vel)
{}

UC_Cursor::~UC_Cursor()
{
}

void UC_Cursor::Init()
{
    Rect rect128 = { 0,0,128,128};
    Rect rect256 = { 0,0,256,256 };


    //Create transform component
    m_pTransform = m_pParent->AddComponent(new EC_Transform(256 + 64, 256 - 64 ));

    m_pTransform->SetScale(0.5f, 0.5f);
    //Create Sprite component
    EC_Sprite* sprite = m_pParent->AddComponent(new EC_Sprite("SS_CURSOR_Y.png", rect256));

    sprite->SetPivotToCenter();
    //Create animator component
    m_pAnimator = m_pParent->AddComponent(new EC_Animator());

    //Load animation info
    m_pAnimator->AddClip("MOVE_RIGHT", 4, Rect(0, 256, 256, 256));
    m_pAnimator->AddClip("MOVE_DOWN", 4, Rect(0, 256 * 2, 256, 256));
    m_pAnimator->AddClip("MOVE_LEFT", 4, Rect(0, 256 * 3, 256, 256));
    m_pAnimator->AddClip("MOVE_UP", 4, Rect(0, 256 * 4, 256, 256));
    m_pAnimator->AddClip("IDLE", 1, Rect(0, 0, 256, 256));

    m_pAnimator->SetSpeed(0.05f);
    m_pAnimator->Play(true);
}

void UC_Cursor::SetVel(float newVel)
{
    m_vel = newVel;
}

void UC_Cursor::Update(float delta)
{
    static float twoSquared = sqrtf(2.0f);
    Point vel(0.0f,0.0f);
    int activeKeys = 0;


    if (InputModule::Get()->GetKeyState(KeyCode::k_W) == KeyState::k_KS_Hold)
        activeKeys++;
    if (InputModule::Get()->GetKeyState(KeyCode::k_A) == KeyState::k_KS_Hold)
        activeKeys++;
    if (InputModule::Get()->GetKeyState(KeyCode::k_S) == KeyState::k_KS_Hold)
        activeKeys++;
    if (InputModule::Get()->GetKeyState(KeyCode::k_D) == KeyState::k_KS_Hold)
        activeKeys++;


    if (InputModule::Get()->GetKeyState(KeyCode::k_D) == KeyState::k_KS_Hold && activeKeys == 1)
    {
        vel.x += m_vel * delta;

        if (m_pAnimator->GetClipName() != std::string("MOVE_RIGHT"))
            m_pAnimator->SetClip("MOVE_RIGHT");
    }
    if (InputModule::Get()->GetKeyState(KeyCode::k_S) == KeyState::k_KS_Hold && activeKeys == 1)
    {
        vel.y += m_vel * delta;

        if (m_pAnimator->GetClipName() != std::string("MOVE_DOWN"))
            m_pAnimator->SetClip("MOVE_DOWN");
    }
    if (InputModule::Get()->GetKeyState(KeyCode::k_A) == KeyState::k_KS_Hold && activeKeys == 1)
    {
         vel.x -= m_vel * delta;

        if (m_pAnimator->GetClipName() != std::string("MOVE_LEFT"))
            m_pAnimator->SetClip("MOVE_LEFT");
    }
    if (InputModule::Get()->GetKeyState(KeyCode::k_W) == KeyState::k_KS_Hold && activeKeys == 1)
    {
         vel.y -= m_vel * delta;

        if (m_pAnimator->GetClipName() != std::string("MOVE_UP"))
            m_pAnimator->SetClip("MOVE_UP");
    }


    if (vel.x != 0.0f && vel.y != 0.0f)
    {
        vel.x = 0;
        vel.y = 0;
    }

    if(vel.x != 0 || vel.y != 0)
        m_pTransform->SetPos(m_pTransform->GetPos().x + vel.x, m_pTransform->GetPos().y + vel.y);

    if (vel.x == 0 && vel.y == 0)
    {
        m_pAnimator->SetClip("IDLE");
        m_pAnimator->Play(false);
    }
    else
        m_pAnimator->Play(true);
}