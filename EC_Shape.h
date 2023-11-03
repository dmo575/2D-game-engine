#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Quad2D
#include <vector>//             std::vector
#include <memory>

class ShapeManager;
class EC_Transform;

class EC_Shape : public Component
{
    friend class ShapeManager;

public:
    EC_Shape();
    virtual ~EC_Shape() override;


    void AddQuad(Quad2D& toAdd);
    bool EditQuad(size_t index, Quad2D& newData);//true if we successfully returned a quad

    //This shape is the final shape that must appear in screen. This shape
    //is altered by the transforms position and scale.
    //Quad2D* GetFinalShape(size_t& outCount);//REMEMBER TO DELETE THIS
    std::unique_ptr<Quad2D[]> GetFinalShape(size_t& outCount);

private:
    virtual void Init() override;

private:
    static ShapeManager* m_pShapeManager;
    EC_Transform* m_pTransform;
    std::vector<Quad2D> m_quads;
};