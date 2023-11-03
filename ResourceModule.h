#pragma once
#include <memory>
#include "CoreStructs.h"

class ResourceModule
{
    friend class Engine;

public:
    static ResourceModule* Get()
    {
        return m_self;
    }

    virtual void* LoadTexture(const char* path) = 0;
    virtual void* LoadFont(const char* path, int size) = 0;
    virtual void* LoadSound(const char* path) = 0;
    virtual void* LoadMusic(const char* path) = 0;


    virtual void UnloadTexture(void* texture) = 0;
    virtual void UnloadFont(void* font) = 0;
    virtual void UnloadSound(void* soundData) = 0;
    virtual void UnloadMusic(void* soundData) = 0;
    virtual void Music_AddVolume(int vol) = 0;
    virtual int Music_GetVolume() = 0;


    virtual int PlaySound(void* soundData, int& soundID) = 0;
    virtual void PlayMusic(void* soundData) = 0;

    virtual void StopSound(int& soundID) = 0;
    virtual void StopMusic() = 0;
    virtual void ResumeMusic() = 0;

    virtual bool MusicPlaying() = 0;

    virtual const char* GetBasePath() = 0;

protected:
    ResourceModule() {}
    virtual ~ResourceModule() {}

    virtual void Init() = 0;
    virtual void DeInit() = 0;

protected:
    static ResourceModule* m_self;
};