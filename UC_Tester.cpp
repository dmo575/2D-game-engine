#include"UC_Tester.h"
#include "UC_Includes.h"
#include <iostream>

UC_Tester::UC_Tester(KeyCode deleteKey, Color c)
    : Component()
    , m_deleteKey(deleteKey)
    , m_color(c)
    , m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_pOtherTransform(nullptr)
{}

UC_Tester::~UC_Tester()
{
    for (size_t i = 0; i < m_others.size(); ++i)
    {
        if ((*m_others[i]))
        {
            (*m_others[i])->DestroySafeRef(&(*m_others[i]));
            delete m_others[i];
        }
    }
    m_pEventSender->TriggerEvent(Event::SHOOTER_OBR_DESTROYED);
}

void UC_Tester::Init()
{
    m_pEventSender = std::make_unique<OEventSender>(m_pParent);
    m_pTransform = m_pParent->GetComponent<EC_Transform>();
    m_pSprite = m_pParent->GetComponent<EC_Sprite>();
}

void UC_Tester::Update(float delta)
{
    if (GameManager::Get()->GetDeltaVel() == 0.0f) return;

    RenderableObject obj
    (
        RenderableObjectType::k_Rect,
        Rect
        (
        m_pSprite->GetWorldRect().x,
        m_pSprite->GetWorldRect().y,
        m_pSprite->GetWorldRect().w,
        m_pSprite->GetWorldRect().h
        ),
        m_color
    );

    PostFrameDrawer::Get()->DrawRenderableObject(obj);

    obj.m_type = RenderableObjectType::k_Line;
    obj.m_data.x = static_cast<int>(m_pTransform->GetPos().x);
    obj.m_data.y = static_cast<int>(m_pTransform->GetPos().y);


    for (Entity** otherEnt : m_others)
    {
        if (*otherEnt)
        {
            EC_Transform* otherT = (*otherEnt)->GetComponent<EC_Transform>();
            obj.m_data.w = static_cast<int>(otherT->GetPos().x-10);
            obj.m_data.h = static_cast<int>(otherT->GetPos().y-10);
            PostFrameDrawer::Get()->DrawRenderableObject(obj);
        }
    }

    if (InputModule::Get()->GetKeyState(m_deleteKey) == KeyState::k_KS_Press)
        m_pParent->Destroy();
}

void UC_Tester::AddTesterToVector(UC_Tester* other)
{
    if (!other) return;

    //Allocate an Eneity* object on the HEAP and get its address sent to m_others[lastIndex].
    m_others.push_back(new Entity*);

    //Access the Entity* in he HEAP, and use it to get a "Safe Reference"
    *(m_others[m_others.size() - 1]) = other->GetParent()->GetSafeRef(&*(m_others[m_others.size() - 1]));

    //The reason we need the Entity* in the HEAP is because we are looking to use this entity
    //reference outside of the scope of this function, if we werent then a normal stack allocated
    //pointer object would be fine. (ColliderManager) uses those in its UpdateCollisions().
}

void UC_Tester::Sub(Event e, IEventListener* listener)
{
    m_pEventSender->Sub(e, listener);
}

void UC_Tester::Unsub(Event e, IEventListener* listener)
{
    m_pEventSender->Unsub(e, listener);
}