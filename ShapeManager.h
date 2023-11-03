#pragma once
#include <vector>//         std::vector

class EC_Shape;
class ShapeManager
{
    friend class Engine;

public:
    ShapeManager();
    ~ShapeManager();

    void AddShape(EC_Shape* shape);
    void RemoveShape(EC_Shape* shape);
    const std::vector<EC_Shape*>* GetShapes() const;

private:
    void Init();

private:
    std::vector<EC_Shape*> m_shapes;
};