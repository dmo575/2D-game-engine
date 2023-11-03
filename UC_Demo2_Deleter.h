#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      ColData

class UC_Demo2_Deleter : public Component
{
public:
    enum class Type {k_AutoDestroy, k_Destroyable, k_Destroys, k_Bomb};

public:
    UC_Demo2_Deleter(Type t);
    virtual ~UC_Demo2_Deleter() override;

    void ActivateBomb(Entity* ent);

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    virtual void OnTriggerEnter(Entity* other, ColData data) override;

public:
    Type m_type;
    Entity* m_pTriggerEnt;
};
