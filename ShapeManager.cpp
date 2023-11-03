#include "ShapeManager.h"
#include "EC_Shape.h"

ShapeManager::ShapeManager()
{
    Init();
}

ShapeManager::~ShapeManager()
{}

void ShapeManager::Init()
{
    EC_Shape::m_pShapeManager = this;
}

void ShapeManager::AddShape(EC_Shape* shape)
{
    m_shapes.push_back(shape);
}

void ShapeManager::RemoveShape(EC_Shape* shape)
{
    for (size_t i = 0; i < m_shapes.size(); ++i)
    {
        if (shape == m_shapes[i])
        {
            m_shapes.erase(m_shapes.begin() + i);
            return;
        }
    }
}

const std::vector<EC_Shape*>* ShapeManager::GetShapes() const
{
    return &m_shapes;
}