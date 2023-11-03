#pragma once
#include <string>


class SaveManager
{
    friend class Engine;

public:
    static SaveManager* Get()
    {
        return m_pSelf;
    }

    bool SaveData(std::string path, std::string fileName, void* dataContainer, std::streamsize dataContainerSize);
    bool LoadData(std::string path, std::string fileName, void* dataContainer, std::streamsize dataContainerSize);

public:
    SaveManager();
    ~SaveManager();
    void Init();

private:
    static SaveManager* m_pSelf;
};