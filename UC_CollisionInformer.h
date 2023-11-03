#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      ColData

class UC_CollisionInformer : public Component
{
public:
    UC_CollisionInformer();
    virtual ~UC_CollisionInformer() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void OnTriggerEnter(Entity* other, ColData data) override;
    virtual void OnTriggerStay(Entity* other, ColData data) override;
    virtual void OnTriggerExit(Entity* other, ColData data) override;
};
