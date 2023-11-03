#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Point
#include <vector>//             std::vector

class EC_Transform;
class UC_Demo5_Building;

class UC_Demo5_HallwayManager : public Component
{
public:
    UC_Demo5_HallwayManager(Point angle);
    virtual ~UC_Demo5_HallwayManager() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Transform* m_pTransform;
    std::vector< UC_Demo5_Building*> m_buildings;
    Point m_angle;
};