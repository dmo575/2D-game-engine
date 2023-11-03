#include "SDL_ResourceModule.h"
#include "SDL_RendererModule.h"
#include "Logger.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>


SDL_ResourceModule::SDL_ResourceModule()
{}

SDL_ResourceModule::~SDL_ResourceModule()
{}

void SDL_ResourceModule::Init()
{
    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 1024);

    Logger::Log(Logger::LOG_LEVEL::k_Info, "SDL_ResourceModule initialized");

    //This should be in the interface code.
    if (m_self == nullptr)
        m_self = this;
}

void SDL_ResourceModule::DeInit()
{
    Mix_CloseAudio();
    Logger::Log(Logger::LOG_LEVEL::k_Info, "SDL_ResourceModule de-initialized");
}

void* SDL_ResourceModule::LoadTexture(const char* path)
{
    SDL_Surface* surface = IMG_Load(path);

    if (!surface)
    {
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("SDL_ResourceModule cannot create surface from \"" + std::string(path) + "\"").c_str());
        return nullptr;
    }

    SDL_RendererModule* ren = (SDL_RendererModule*)SDL_RendererModule::Get();

    if (!ren)
    {
        Logger::Log(Logger::LOG_LEVEL::k_Error, "SDL_ResourceModule cannot find the SDL_Renderer");
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren->GetRenderer(), surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("SDL_RensourceModule cannot create texture from \"" + std::string(path) + "\"").c_str());
        return nullptr;
    }

    return texture;
}

void* SDL_ResourceModule::LoadFont(const char* path, int size)
{
    TTF_Font* font = TTF_OpenFont(path, size);

    if (!font)
    {
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("SDL_ResourceModule cannot create font from \"" + std::string(path) + "\"").c_str());
        return nullptr;
    }

    return font;
}

void* SDL_ResourceModule::LoadSound(const char* path)
{
    Mix_Chunk* soundData = Mix_LoadWAV(path);

    if (!soundData)
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("SDL_ResourceModule cannot create soundData from \"" + std::string(path) + "\"").c_str());

    return soundData;
}

void* SDL_ResourceModule::LoadMusic(const char* path)
{
    Mix_Music* musicData = Mix_LoadMUS(path);
    
    if (!musicData)
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("SDL_ResourceModule cannot create musicData from \"" + std::string(path) + "\"").c_str());
    
    return musicData;
}

void SDL_ResourceModule::UnloadTexture(void* texture)
{
    SDL_DestroyTexture((SDL_Texture*)texture);
}

void SDL_ResourceModule::UnloadFont(void* font)
{
    TTF_CloseFont((TTF_Font*)font);
}

void SDL_ResourceModule::UnloadSound(void* soundData)
{
    Mix_FreeChunk((Mix_Chunk*)soundData);
}

void SDL_ResourceModule::UnloadMusic(void* soundData)
{
    Mix_FreeMusic((Mix_Music*)soundData);
}

int SDL_ResourceModule::PlaySound(void* soundData, int& channelID)
{
    return Mix_PlayChannel(channelID, (Mix_Chunk*)soundData, 0);
}

void SDL_ResourceModule::PlayMusic(void* soundData)
{
    int errorCode = Mix_PlayMusic((Mix_Music*)soundData, 0);
    
    if (errorCode != 0)
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("SDL_ResourceModule cannot play music").c_str());
}

void SDL_ResourceModule::StopSound(int& soundID)
{
    Mix_HaltChannel(soundID);
}

void SDL_ResourceModule::StopMusic()
{
    Mix_HaltMusic();
}

void SDL_ResourceModule::ResumeMusic()
{
    Mix_ResumeMusic();
}

bool SDL_ResourceModule::MusicPlaying()
{
    if (Mix_PlayingMusic())
        return true;
    else
        return Mix_PausedMusic();
}

void SDL_ResourceModule::Music_AddVolume(int vol)
{
    int finalVol = Mix_VolumeMusic(-1) + vol;

    if (finalVol < 0)
        finalVol = 0;

    Mix_VolumeMusic(finalVol);
}

int SDL_ResourceModule::Music_GetVolume()
{
    return Mix_VolumeMusic(-1);
}


std::shared_ptr<SDL_Texture*> SDL_ResourceModule::CheckList(const char* fileID)
{
    for (std::pair<const char*, std::shared_ptr<SDL_Texture*>> pair : m_surfaces)
    {
        if (pair.first == fileID)
            return pair.second;
    }

    return nullptr;
}

const char* SDL_ResourceModule::GetBasePath()
{
    return SDL_GetBasePath();
}