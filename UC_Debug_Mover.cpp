#include "UC_Debug_Mover.h"
#include "UC_Includes.h"

UC_Debug_Mover::UC_Debug_Mover(EC_Transform* t, Point dir, float vel)
    : m_t(t)
    , m_dir(dir)
    , m_vel(vel)
{}

UC_Debug_Mover::~UC_Debug_Mover()
{}

void UC_Debug_Mover::Init()
{}

void UC_Debug_Mover::Update(float delta)
{
    m_t->AddPos(m_dir * m_vel * delta);
}