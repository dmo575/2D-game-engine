#include "UC_Demo2_Player.h"
#include "UC_Includes.h"
#include "UC_Demo2_Deleter.h"

UC_Demo2_Player::UC_Demo2_Player()
    : Component()
{}

UC_Demo2_Player::~UC_Demo2_Player()
{}

void UC_Demo2_Player::Init()
{}

void UC_Demo2_Player::Update(float delta)
{}

void UC_Demo2_Player::OnTriggerEnter(Entity* other, ColData data)
{
    Logger::Log("Player - Trigger ENTER");

    if (other != nullptr)
    {
        auto otherComp = other->GetComponent<UC_Demo2_Deleter>();

        if (!otherComp) return;

        if (otherComp->m_type == UC_Demo2_Deleter::Type::k_Destroyable)
            other->Destroy();
        else if (otherComp->m_type == UC_Demo2_Deleter::Type::k_Bomb)
        {
            otherComp->ActivateBomb(m_pParent);
            other->Destroy();
        }
    }
}

void UC_Demo2_Player::OnTriggerStay(Entity* other, ColData data)
{
    Logger::Log("Player - Trigger STAY");
}

void UC_Demo2_Player::OnTriggerExit(Entity* other, ColData data)
{
    Logger::Log("Player - Trigger EXIT");
}

void UC_Demo2_Player::OnCollision(Entity* other, ColData data)
{
    Logger::Log("Player - Collision DETECTED");
}