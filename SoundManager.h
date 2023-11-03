#pragma once
#include <vector>
#include "CoreStructs.h"
#include <string>

class EC_Sound;

class SoundManager
{
    friend class Engine;
    friend class EC_Sound;

private:
    struct SoundData
    {
        const char* m_fileName;
        void* data;
        unsigned int m_refCount;
    };

public:
    static SoundManager* Get()
    {
        return m_pSelf;
    }
    //This can be called by anyone
    void PlayMusic(const char* fileName);
    void StopMusic(bool freeFromMem);
    void ResumeMusic();
    bool MusicPlaying();
    void Music_AddVolume(int vol);

private:
    SoundManager();
    ~SoundManager();
    void Init();

    //This updates reference counts and returns data
    void* UpdateSound(EC_Sound* comp);
    void DereferenceSound(EC_Sound* comp);

    //play a sound
    void PlaySound(EC_Sound* comp);
    void StopSound(EC_Sound* comp);

private:
    static SoundManager* m_pSelf;
    std::vector<SoundData> m_sounds;
    void* m_pMusicData;
    const char* m_musicFileName;
};