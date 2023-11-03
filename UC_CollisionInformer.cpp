#include "UC_CollisionInformer.h"
#include "UC_Includes.h"

UC_CollisionInformer::UC_CollisionInformer()
{}

UC_CollisionInformer::~UC_CollisionInformer()
{}

void UC_CollisionInformer::Init()
{}

void UC_CollisionInformer::Update(float delta)
{}

void UC_CollisionInformer::OnTriggerEnter(Entity* other, ColData data)
{
    Logger::Log("UC_CollisionInformer -- Trigger ENTER");
}

void UC_CollisionInformer::OnTriggerStay(Entity* other, ColData data)
{
    Logger::Log("UC_CollisionInformer -- Trigger STAY");
}

void UC_CollisionInformer::OnTriggerExit(Entity* other, ColData data)
{
    Logger::Log("UC_CollisionInformer -- Trigger EXIT");
}