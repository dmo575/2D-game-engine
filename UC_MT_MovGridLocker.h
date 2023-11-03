#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Point, ColData

class UC_WASDController;
class EC_Transform;

class UC_MT_MovGridLocker : public Component
{
public:
    UC_MT_MovGridLocker(EC_Transform* t, UC_WASDController* c);
    virtual ~UC_MT_MovGridLocker() override;

    Point GetCurrDir();
    void SetGridValue(Point grid);

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void OnCollision(Entity* other, ColData data) override;

    void UpdateTargetPos(Point& newDir);
    bool DestinationReached(Point& frameMov);

private:
    EC_Transform* m_pTransform;
    UC_WASDController* m_pController;
    Point m_gridData;
    Point m_startPos;
    Point m_targetPos;
    Point m_currDir;
    Point m_currHostTile;
    bool m_moving;
};