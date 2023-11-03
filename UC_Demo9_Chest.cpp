#include "UC_Demo9_Chest.h"
#include "UC_Includes.h"
#include "UC_Demo9_Selectable.h"

int UC_Demo9_Chest::m_totalChests = 0;
int UC_Demo9_Chest::m_openedChests = 0;

UC_Demo9_Chest::UC_Demo9_Chest(UC_Demo9_Selectable* s, Point startPos)
    : m_pTransform(nullptr)
    , m_pSprite(nullptr)
    , m_pSelectableTransform(nullptr)
    , m_pSelectable(s)
    , m_interactionDist(20.0f)
    , m_startPos(startPos)
    , m_opened(false)
{}

UC_Demo9_Chest::~UC_Demo9_Chest()
{}

void UC_Demo9_Chest::Init()
{
    m_totalChests++;
    Rect srcRect{0,0,128/2, 29};

    m_pTransform = m_pParent->AddComponent(new EC_Transform);
    m_pSprite = m_pParent->AddComponent(new EC_Sprite);

    m_pTransform->SetPos(m_startPos);

    m_pSprite->SetSourceImg("SP_Chest.png", srcRect);
    m_pSprite->SetPivotToCenter();

    m_pSelectableTransform = m_pSelectable->GetParent()->GetComponent<EC_Transform>();
}

void UC_Demo9_Chest::Update(float delta)
{
    if (m_pSelectable->GetState() == UC_Demo9_Selectable::SelectableState::k_OffGrab)
    {
        float dist = MathModule::Distance(m_pSelectableTransform->GetPos(), m_pTransform->GetPos());

        if (dist <= m_interactionDist)
            OpenChest();
    }
}

void UC_Demo9_Chest::OpenChest()
{

    if (m_opened) return;

    m_opened = true;
    Rect srcRect{ 128 / 2,0,128 / 2, 29 };

    m_openedChests++;
    m_pSprite->SetSourceRect(srcRect);

    if (m_totalChests == m_openedChests)
        LevelManager::Get()->QueueLoadLevel("demo_6_YouWin");
}