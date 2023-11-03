#pragma once
#include "ResourceModule.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <utility>

class SDL_ResourceModule : public ResourceModule
{
    friend class Engine;
    using pair = std::pair <const char*, std::shared_ptr<SDL_Texture*>>;

public:
    virtual void* LoadTexture(const char* path) override;
    virtual void* LoadFont(const char* path, int size) override;
    virtual void* LoadSound(const char* path) override;
    virtual void* LoadMusic(const char* path) override;


    virtual void UnloadTexture(void* texture) override;
    virtual void UnloadFont(void* texture) override;
    virtual void UnloadSound(void* soundData) override;
    virtual void UnloadMusic(void* soundData) override;
    virtual void Music_AddVolume(int vol) override;
    virtual int Music_GetVolume() override;

    virtual int PlaySound(void* soundData, int& channelID) override;
    virtual void PlayMusic(void* soundData) override;

    virtual void StopSound(int& soundID) override;
    virtual void StopMusic() override;
    virtual void ResumeMusic() override;


    virtual bool MusicPlaying() override;

    virtual const char* GetBasePath();

protected:
    SDL_ResourceModule();
    virtual ~SDL_ResourceModule() override;

    virtual void Init() override;
    virtual void DeInit() override;

private:
    std::shared_ptr<SDL_Texture*> CheckList(const char* fileID);

private:
    std::vector<pair> m_surfaces;
};