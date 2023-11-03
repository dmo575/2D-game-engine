#include "TextManager.h"
#include "EC_Text.h"
#include "ResourceModule.h"

TextManager::TextManager()
{
    Init();
}

TextManager::~TextManager()
{}

void TextManager::Init()
{
    EC_Text::m_pTextManager = this;
}

void TextManager::AddTextComp(EC_Text* text)
{
    m_textComps.push_back(text);
}

void TextManager::RemoveTextComp(EC_Text* text)
{
    for (size_t i = 0; i < m_textComps.size(); ++i)
    {
        if (m_textComps[i] == text)
        {
            UnloadFont(m_textComps[i]->m_font);
            m_textComps.erase(m_textComps.begin() + i);
            return;
        }
    }
}

const std::vector<EC_Text*>* TextManager::GetTexts()
{
    return &m_textComps;
}

void* TextManager::LoadFont(const char* relativePath, unsigned int size)
{
    std::string finalPath = ResourceModule::Get()->GetBasePath() +std::string("Resources/") + std::string(relativePath);
    return ResourceModule::Get()->LoadFont(finalPath.c_str(), size);
}

void TextManager::UnloadFont(void* font)
{
    if(font)
        ResourceModule::Get()->UnloadFont(font);
}