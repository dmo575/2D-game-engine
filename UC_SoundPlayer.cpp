#include "UC_SoundPlayer.h"
#include "UC_Includes.h"

UC_SoundPlayer::UC_SoundPlayer(float countDown, const char* soundToPlay)
    : Component()
    , m_pSound(nullptr)
    , m_countDown(countDown)
    , m_soundToPlay(soundToPlay)
{}

UC_SoundPlayer::~UC_SoundPlayer()
{}

void UC_SoundPlayer::Init()
{
    m_pSound = m_pParent->AddComponent(new EC_Sound);
    m_pSound->SetSound(m_soundToPlay);
}

void UC_SoundPlayer::Update(float delta)
{
    static float var = 0.0f;

    var += delta;

    if (var >= m_countDown)
    {
        m_pSound->Play();
        var -= m_countDown;
    }

    if (InputModule::Get()->GetKeyState(KeyCode::k_Space) == KeyState::k_KS_Press)
        m_pSound->Stop();
}