#pragma once
#include <vector>
#include "CoreStructs.h"

class EC_Text;
class TextManager
{
    friend class Engine;

public:
    void AddTextComp(EC_Text* text);
    void RemoveTextComp(EC_Text* text);
    void* LoadFont(const char* path, unsigned int size);
    void UnloadFont(void* font);

    const std::vector<EC_Text*>* GetTexts();

private:
    TextManager();
    ~TextManager();
    void Init();

private:
    std::vector<EC_Text*> m_textComps;
};