#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      ColData

class UC_Demo2_Player : public Component
{
public:
    UC_Demo2_Player();
    virtual ~UC_Demo2_Player() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void OnTriggerEnter(Entity* other, ColData data) override;
    virtual void OnTriggerStay(Entity* other, ColData data) override;
    virtual void OnTriggerExit(Entity* other, ColData data) override;
    virtual void OnCollision(Entity* other, ColData data) override;
};