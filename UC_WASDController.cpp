#include "UC_WASDController.h"
#include "UC_Includes.h"
#include <math.h>//         abs

UC_WASDController::UC_WASDController(float vel, DirType dir)
    : Component()
    , m_vel(vel)
    , m_pTransform(nullptr)
    , m_dirType(dir)
    , m_currDir(0,0)
    , m_applyMov(true)
{
    for (size_t i = 0; i < 4; ++i)
        m_keysOrder[i] = KeyCode::k_Unknown;
}

UC_WASDController::~UC_WASDController()
{
}

void UC_WASDController::Init()
{
    m_pTransform = m_pParent->GetComponent<EC_Transform>();
}

void UC_WASDController::SetVel(float newVel)
{
    m_vel = newVel;
}

void UC_WASDController::Update(float delta)
{

    if (InputModule::Get()->GetKeyState(KeyCode::k_W) == KeyState::k_KS_Release)
        RemoveFromKeysOrder(KeyCode::k_W);
    if (InputModule::Get()->GetKeyState(KeyCode::k_A) == KeyState::k_KS_Release)
        RemoveFromKeysOrder(KeyCode::k_A);
    if (InputModule::Get()->GetKeyState(KeyCode::k_S) == KeyState::k_KS_Release)
        RemoveFromKeysOrder(KeyCode::k_S);
    if (InputModule::Get()->GetKeyState(KeyCode::k_D) == KeyState::k_KS_Release)
        RemoveFromKeysOrder(KeyCode::k_D);

    if (InputModule::Get()->GetKeyState(KeyCode::k_W) == KeyState::k_KS_Press)
        AddToKeysOrder(KeyCode::k_W);
    if (InputModule::Get()->GetKeyState(KeyCode::k_A) == KeyState::k_KS_Press)
        AddToKeysOrder(KeyCode::k_A);
    if (InputModule::Get()->GetKeyState(KeyCode::k_S) == KeyState::k_KS_Press)
        AddToKeysOrder(KeyCode::k_S);
    if (InputModule::Get()->GetKeyState(KeyCode::k_D) == KeyState::k_KS_Press)
        AddToKeysOrder(KeyCode::k_D);



    if (m_dirType == DirType::EIGHT)
        UpdateWASD8(delta);
    else if (m_dirType == DirType::FOUR)
        UpdateWASD4(delta);
}

void UC_WASDController::UpdateWASD8(float delta)
{
    static float twoSquared = sqrtf(2.0f);
    float diagonalVel = m_vel / twoSquared;
    Point vel;


    if (InputModule::Get()->GetKeyState(KeyCode::k_D) == KeyState::k_KS_Hold)
    {

        if ((InputModule::Get()->GetKeyState(KeyCode::k_W) == KeyState::k_KS_Hold) ^
            (InputModule::Get()->GetKeyState(KeyCode::k_S) == KeyState::k_KS_Hold))
            vel.x += diagonalVel * delta;
        else
            vel.x += m_vel * delta;
    }
    if (InputModule::Get()->GetKeyState(KeyCode::k_S) == KeyState::k_KS_Hold)
    {

        if ((InputModule::Get()->GetKeyState(KeyCode::k_D) == KeyState::k_KS_Hold) ^
            (InputModule::Get()->GetKeyState(KeyCode::k_A) == KeyState::k_KS_Hold))
            vel.y += diagonalVel * delta;
        else
            vel.y += m_vel * delta;
    }
    if (InputModule::Get()->GetKeyState(KeyCode::k_A) == KeyState::k_KS_Hold)
    {

        if ((InputModule::Get()->GetKeyState(KeyCode::k_W) == KeyState::k_KS_Hold) ^
            (InputModule::Get()->GetKeyState(KeyCode::k_S) == KeyState::k_KS_Hold))
            vel.x -= diagonalVel * delta;
        else
            vel.x -= m_vel * delta;
    }
    if (InputModule::Get()->GetKeyState(KeyCode::k_W) == KeyState::k_KS_Hold)
    {

        if ((InputModule::Get()->GetKeyState(KeyCode::k_D) == KeyState::k_KS_Hold) ^
            (InputModule::Get()->GetKeyState(KeyCode::k_A) == KeyState::k_KS_Hold))
            vel.y -= diagonalVel * delta;
        else
            vel.y -= m_vel * delta;
    }

    if (vel.x != 0)
        m_currDir.x = vel.x > 0.0f ? 1.0f : -1.0f;
    else
        m_currDir.x = 0;

    if (vel.y != 0)
        m_currDir.y = vel.y > 0.0f ? 1.0f : -1.0f;
    else
        m_currDir.y = 0.0f;

    if(m_applyMov)
        m_pTransform->SetPos(m_pTransform->GetPos().x + vel.x, m_pTransform->GetPos().y + vel.y);
}

void UC_WASDController::UpdateWASD4(float delta)
{
    Point vel(0.0f,0.0f);

    KeyCode lastKey = GetLastestKey();

    if (lastKey == KeyCode::k_W && InputModule::Get()->GetKeyState(KeyCode::k_S) == KeyState::k_KS_Null)
        vel.y -= m_vel * delta;
    else if (lastKey == KeyCode::k_A && InputModule::Get()->GetKeyState(KeyCode::k_D) == KeyState::k_KS_Null)
        vel.x -= m_vel * delta;
    else if (lastKey == KeyCode::k_S && InputModule::Get()->GetKeyState(KeyCode::k_W) == KeyState::k_KS_Null)
        vel.y += m_vel * delta;
    else if (lastKey == KeyCode::k_D && InputModule::Get()->GetKeyState(KeyCode::k_A) == KeyState::k_KS_Null)
        vel.x += m_vel * delta;

    if (vel.x != 0)
        m_currDir.x = vel.x > 0.0f ? 1.0f : -1.0f;
    else
        m_currDir.x = 0.0f;

    if (vel.y != 0)
        m_currDir.y = vel.y > 0.0f ? 1.0f : -1.0f;
    else
        m_currDir.y = 0.0f;

    if(m_applyMov)
        m_pTransform->SetPos(m_pTransform->GetPos().x + vel.x, m_pTransform->GetPos().y + vel.y);
}

void UC_WASDController::AddToKeysOrder(KeyCode key)
{
    for (int i = 3; i >= 0; --i)
    {
        if (i > 0)
            m_keysOrder[static_cast<size_t>(i)] = m_keysOrder[static_cast<size_t>(i) - 1];
        else
            m_keysOrder[static_cast<size_t>(i)] = key;
    }
}

void UC_WASDController::RemoveFromKeysOrder(KeyCode key)
{
    for (int i = 0; i < 4; ++i)
    {
        if (m_keysOrder[static_cast<size_t>(i)] == key)
        {
            RemoveSpaceFromKeysOrder(i);
            return;
        }
    }
}

KeyCode UC_WASDController::GetLastestKey()
{
    for (int i = 0; i < 4; ++i)
    {
        if (m_keysOrder[static_cast<size_t>(i)] != KeyCode::k_Unknown)
            return m_keysOrder[static_cast<size_t>(i)];
    }

    return KeyCode::k_Unknown;
}

void UC_WASDController::RemoveSpaceFromKeysOrder(int spaceIndex)
{
    for (int i = spaceIndex; i < 3; ++i)
    {
        m_keysOrder[static_cast<size_t>(i)] = m_keysOrder[static_cast<size_t>(i + 1)];
    }
    m_keysOrder[3] = KeyCode::k_Unknown;
}

Point UC_WASDController::GetCurrDir()
{
    return m_currDir;
}

void UC_WASDController::ApplyMovement(bool value)
{
    m_applyMov = value;
}

float UC_WASDController::GetVel()
{
    return m_vel;
}