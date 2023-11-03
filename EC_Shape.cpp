#include "EC_Shape.h"
#include "EC_Includes.h"
#include "EC_Transform.h"
#include "ShapeManager.h"


ShapeManager* EC_Shape::m_pShapeManager = nullptr;

EC_Shape::EC_Shape()
    : Component()
    , m_pTransform(nullptr)
{}

EC_Shape::~EC_Shape()
{
    m_pShapeManager->RemoveShape(this);
}

void EC_Shape::Init()
{
    m_pTransform = m_pParent->GetComponent<EC_Transform>();


    //NOTE: This means if we dont have a transform, we never add theshape to the shapeManager
    if (!m_pTransform)
    {
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("Missing EC_Transform for EC_Shape in \"" + m_pParent->GetName() + std::string("\"")).c_str());
        return;
    }
    m_pShapeManager->AddShape(this);
}

void EC_Shape::AddQuad(Quad2D& toAdd)
{
    m_quads.push_back(toAdd);//this should be a copy of toAdd.
}


std::unique_ptr<Quad2D[]> EC_Shape::GetFinalShape(size_t& outCount)
{

    auto q = std::make_unique<Quad2D[]>(m_quads.size());

    Point origin = m_pTransform->GetPos();
    float distance;
    Point direction;
    Point scale = m_pTransform->GetScale();
    Point finalPoint = Point();

    for (size_t quadsI = 0; quadsI < m_quads.size(); ++quadsI)
    {
        for (size_t i = 0; i < 4; ++i)
        {
            distance = MathModule::Distance(Point(0, 0), m_quads[quadsI].p[i]);
            direction = MathModule::GetDirection(Point(0, 0), m_quads[quadsI].p[i]);

            //Add the length
            finalPoint = direction * distance;
            //Scale the length

            finalPoint.x *= scale.x;
            finalPoint.y *= scale.y;

            //Sum the length to the origin point so we get the final point
            finalPoint += origin;

            //If the Point contains a nan value, set that value to zero
            if (finalPoint.x != finalPoint.x)
                finalPoint.x = 0.0f;
            if (finalPoint.y != finalPoint.y)
                finalPoint.y = 0.0f;

            //save it
            q[quadsI].p[i] = finalPoint;
        }
    }

    outCount = m_quads.size();
    return q;
}

bool EC_Shape::EditQuad(size_t index, Quad2D& newData)
{
    if (m_quads.size() - 1 < index) return false;

    m_quads[index] = newData;
    return true;
}