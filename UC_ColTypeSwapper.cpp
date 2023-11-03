#include "UC_ColTypeSwapper.h"
#include "UC_Includes.h"

UC_ColTypeSwapper::UC_ColTypeSwapper(KeyCode swapKey)
    : Component()
    , m_swapKey(swapKey)
    , m_col(nullptr)

{}

UC_ColTypeSwapper::~UC_ColTypeSwapper()
{}

void UC_ColTypeSwapper::Init()
{
    m_col = m_pParent->GetComponent<EC_Collider>();
}

void UC_ColTypeSwapper::Update(float delta)
{

    if (InputModule::Get()->GetKeyState(m_swapKey) == KeyState::k_KS_Press)
        m_col->SetTrigger(!m_col->IsTrigger());
}