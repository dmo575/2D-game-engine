#include "SaveManager.h"
#include "Logger.h"
//#include <direct.h>
#include <fstream>
#include <iostream>

SaveManager* SaveManager::m_pSelf = nullptr;

SaveManager::SaveManager()
{
    Init();
}

SaveManager::~SaveManager()
{}

void SaveManager::Init()
{
    if (!m_pSelf)
        m_pSelf = this;
}

bool SaveManager::SaveData(std::string path, std::string fileName, void* dataContainer, std::streamsize dataContainerSize)
{
    /*
    //Create the place where the save data will go if it doesnt exist yet
    _mkdir(path.c_str());

    std::string fullPath = path + "\\" + fileName;

    std::ofstream file;

    //Try to open the file, if it doesn't try to create it and open it
    file.open(fullPath.c_str(), std::ios::out);

    //If for some reason we could not get to the point where we got an opened file, then:
    if (!file.is_open())
    {
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("SaveManager cannot open \"" + fullPath + "\"").c_str());
        return false;
    }

    file.write((char*)dataContainer, dataContainerSize);


    file.close();


    Logger::Log(Logger::LOG_LEVEL::k_Info, "Game data saved successfully.");*/
    return true;
}

bool SaveManager::LoadData(std::string path, std::string fileName, void* dataContainer, std::streamsize dataContainerSize)
{
    std::string fullPath = path + "/" + fileName;

    std::ifstream file;

    file.open(fullPath.c_str(), std::ios::in);

    //If for some reason we could not get to the point where we got an opened file, then:
    if (!file.is_open())
    {
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("SaveManager cannot open \"" + fullPath + "\"").c_str());
        return false;
    }

    file.read((char*)dataContainer, dataContainerSize);

    file.close();

    Logger::Log(Logger::LOG_LEVEL::k_Info, "Game data loaded successfully.");
    return true;
}