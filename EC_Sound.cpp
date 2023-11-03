#include "EC_Sound.h"
#include "SoundManager.h"

SoundManager* EC_Sound::m_pSoundManager = nullptr;

EC_Sound::EC_Sound()
    : Component()
    , m_pSoundData(nullptr)
    , m_channelID(-1)
{}

EC_Sound::~EC_Sound()
{
    m_pSoundManager->DereferenceSound(this);
}

void EC_Sound::SetSound(const char* fileName)
{
    m_soundFileName = fileName;
    m_pSoundData = m_pSoundManager->UpdateSound(this);
}

void EC_Sound::Play()
{
    m_pSoundManager->PlaySound(this);
}

void EC_Sound::Stop()
{
    m_pSoundManager->StopSound(this);
}