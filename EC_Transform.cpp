#include "EC_Transform.h"

EC_Transform::EC_Transform(float x, float y)
    : Component()
    , m_pos(x, y)
    , m_scale(1.0f, 1.0f)
{}

EC_Transform::EC_Transform()
    : EC_Transform(0.0f, 0.0f)
{}

EC_Transform::~EC_Transform()
{}

void EC_Transform::Init()
{}

void EC_Transform::Update(float delta)
{}

Point EC_Transform::GetPos()
{
    return m_pos;
}

void EC_Transform::SetPos(float x, float y)
{
    m_pos.x = x;
    m_pos.y = y;
}

void EC_Transform::SetPos(int x, int y)
{
    m_pos.x = static_cast<float>(x);
    m_pos.y = static_cast<float>(y);
}

void EC_Transform::SetPos(const Point& newPos)
{
    m_pos.x = newPos.x;
    m_pos.y = newPos.y;
}

void EC_Transform::AddPos(float x, float y)
{
    SetPos(m_pos.x + x, m_pos.y + y);
}

void EC_Transform::AddPos(Point toAdd)
{
    SetPos(m_pos.x + toAdd.x, m_pos.y + toAdd.y);
}

Point EC_Transform::GetScale()
{
    return m_scale;
}

void EC_Transform::SetScale(float x, float y)
{
    m_scale.x = x;
    m_scale.y = y;
}

void EC_Transform::SetScale(const Point& newPos)
{
    m_scale.x = newPos.x;
    m_scale.y = newPos.y;
}

void EC_Transform::AddScale(float x, float y)
{
    SetScale(m_scale.x + x, m_scale.y + y);
}