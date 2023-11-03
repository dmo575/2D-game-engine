#include "UC_Demo2_Deleter.h"
#include "UC_Includes.h"

UC_Demo2_Deleter::UC_Demo2_Deleter(Type t)
    : m_type(t)
    , m_pTriggerEnt(nullptr)
    , Component()
{

}

UC_Demo2_Deleter::~UC_Demo2_Deleter()
{
    if (m_type == Type::k_Bomb && m_pTriggerEnt)
        m_pTriggerEnt->Destroy();
}

void UC_Demo2_Deleter::Init()
{

}

void UC_Demo2_Deleter::Update(float delta)
{

}

void UC_Demo2_Deleter::OnTriggerEnter(Entity* other, ColData data)
{
    if (other == nullptr) return;//This should never happen but just in case.

    switch (m_type)
    {
    case UC_Demo2_Deleter::Type::k_AutoDestroy:
        m_pParent->Destroy();
        break;
    case UC_Demo2_Deleter::Type::k_Destroyable:
        //NA
        break;
    case UC_Demo2_Deleter::Type::k_Destroys:
        other->Destroy();
        break;
    default:
        break;
    }
}

void UC_Demo2_Deleter::ActivateBomb(Entity* ent)
{
    m_pTriggerEnt = ent->GetSafeRef(&m_pTriggerEnt);
}
