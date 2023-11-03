#pragma once
#include "Component.h"

class EC_Sound;

class UC_SoundPlayer : public Component
{
public:
    UC_SoundPlayer(float m_countDown, const char* soundToPlay);
    virtual ~UC_SoundPlayer() override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Sound* m_pSound;
    float m_countDown;
    const char* m_soundToPlay;
};