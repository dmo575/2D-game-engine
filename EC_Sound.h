#pragma once
#include "Component.h"
#include <string>

class SoundManager;

class EC_Sound : public Component
{
    friend class SoundManager;
public:
    EC_Sound();
    virtual ~EC_Sound() override;
    void SetSound(const char* fileName);
    void Play();
    void Stop();

private:
    static SoundManager* m_pSoundManager;
    void* m_pSoundData;
    int m_channelID;
    std::string m_soundFileName;
};