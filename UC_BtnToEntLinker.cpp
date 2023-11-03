#include <cstring>
#include "UC_BtnToEntLinker.h"
#include "UC_Includes.h"
#include <vector>
#include "UC_Final_GameManager.h"

UC_BtnToEntLinker::UC_BtnToEntLinker()
{}

UC_BtnToEntLinker::~UC_BtnToEntLinker()
{}


//This only works with UC_Final_GameManager
bool UC_BtnToEntLinker::LinkBtnToEnt(std::string entName, EC_Button* btn, Event e)
{
    Entity* ent = nullptr;
    auto ents = m_entityManager->GetEntities();

    for (auto e : ents)
    {
        if (strcmp(e->GetName().c_str(), entName.c_str()) == 0)
        {
            ent = e;
            break;
        }
    }

    if (!ent) return false;

    auto listener = ent->GetComponent<UC_Final_GameManager>();
    IEventListener* l = (IEventListener*)listener;

    if (!listener) return false;

    btn->Sub(e, l);

    return true;
}