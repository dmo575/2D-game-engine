#include <cstring>
#include "SoundManager.h"
#include "ResourceModule.h"
#include "EC_Sound.h"

SoundManager* SoundManager::m_pSelf = nullptr;

SoundManager::SoundManager()
    : m_pMusicData(nullptr)
    , m_musicFileName("")
{
    Init();
}

SoundManager::~SoundManager()
{}

void SoundManager::Init()
{
    if (m_pSelf == nullptr)
    {
        m_pSelf = this;
        EC_Sound::m_pSoundManager = this;
    }
}

void* SoundManager::UpdateSound(EC_Sound* comp)
{
    //This checks if this var is already holding
    DereferenceSound(comp);

    //Get the path
    std::string path = ResourceModule::Get()->GetBasePath() + std::string("Resources/Sound/") + comp->m_soundFileName;

    //If we have that sound loaded, update count and return it
    for (size_t i = 0; i < m_sounds.size(); ++i)
    {
        if (strcmp(m_sounds[i].m_fileName, comp->m_soundFileName.c_str()) == 0)
        {
            m_sounds[i].m_refCount++;
            return m_sounds[i].data;
        }
    }

    //Here means we dont have the sound loaded.

    //load sound attempt
    void* sound = ResourceModule::Get()->LoadSound(path.c_str());

    //if sound is null the path was incorrect (error msg sent already by ResourceManager)
    if (!sound)
    {
        comp->m_soundFileName = "NO_SOUND";
        return nullptr;
    }

    //create new soundData for new sound
    SoundData newSound;
    newSound.m_fileName = comp->m_soundFileName.c_str();
    newSound.data = sound;
    newSound.m_refCount = 1;

    //push it
    m_sounds.push_back(newSound);

    //return sound pointer
    return sound;
}

void SoundManager::DereferenceSound(EC_Sound* comp)
{
    if (!comp->m_pSoundData) return;

    StopSound(comp);

    for (size_t i = 0; i < m_sounds.size(); ++i)
    {
        if (strcmp(m_sounds[i].m_fileName, comp->m_soundFileName.c_str()) == 0)
        {
            m_sounds[i].m_refCount--;

            if (m_sounds[i].m_refCount == 0)
            {
                ResourceModule::Get()->UnloadSound(m_sounds[i].data);
                m_sounds.erase(m_sounds.begin() + i);
                return;
            }
        }
    }
}

void SoundManager::PlaySound(EC_Sound* comp)
{
    int channelID = ResourceModule::Get()->PlaySound(comp->m_pSoundData, comp->m_channelID);
    comp->m_channelID = channelID;
}

void SoundManager::StopSound(EC_Sound* comp)
{
    if (comp->m_channelID != -1)
    {
        ResourceModule::Get()->StopSound(comp->m_channelID);
        comp->m_channelID = -1;//I am assuming that stopping a sound makes the channel that was
        //being used by that chunk to go availible again.
    }
}

void SoundManager::PlayMusic(const char* fileName)
{
    //If current music is different, unload
    if (strcmp(m_musicFileName, fileName) != 0 && m_pMusicData)
        ResourceModule::Get()->UnloadMusic(m_pMusicData);

    std::string path = ResourceModule::Get()->GetBasePath() + std::string("Resources/Sound/") + fileName;

    //load music attempt
    void* newMusic = ResourceModule::Get()->LoadMusic(path.c_str());

    //if load failed, return
    if (!newMusic) return;

    //play music
    ResourceModule::Get()->PlayMusic(newMusic);

    //update vars
    m_pMusicData = newMusic;
    m_musicFileName = fileName;
}

void SoundManager::ResumeMusic()
{
    ResourceModule::Get()->ResumeMusic();
}

void SoundManager::StopMusic(bool freeFromMem)
{
    if (!m_pMusicData) return;

    int none = 0;
    if (freeFromMem)
        ResourceModule::Get()->UnloadMusic(m_pMusicData);
    else
        ResourceModule::Get()->StopMusic();
}

bool SoundManager::MusicPlaying()
{
    return ResourceModule::Get()->MusicPlaying();
}

void SoundManager::Music_AddVolume(int vol)
{
    ResourceModule::Get()->Music_AddVolume(vol);
}