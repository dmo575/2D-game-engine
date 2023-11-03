#pragma once
#include "Component.h"

class UC_Final_GameManager;
class EC_Text;
class UC_Final_SelectedLevelDisplayer : public Component
{
public:
    UC_Final_SelectedLevelDisplayer(Point pos);
    virtual ~UC_Final_SelectedLevelDisplayer() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    UC_Final_GameManager* m_pGameManager;
    EC_Text* m_pLVL_txt;
    EC_Text* m_pLevel_txt;
    EC_Text* m_pTIME_txt;
    EC_Text* m_pTime_txt;
    EC_Text* m_pSAVES_txt;
    EC_Text* m_pSaves_txt;
    Point m_pivotPos;
};