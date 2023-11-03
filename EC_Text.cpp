#include "EC_Text.h"
#include "TextManager.h"
#include "EC_Includes.h"
#include "EC_Transform.h"

TextManager* EC_Text::m_pTextManager = nullptr;

EC_Text::EC_Text()
    : Component()
    , m_pTransform(nullptr)
    , m_text("")
    , m_size(0)
    , m_color(0,0,0,0)
    , m_font(nullptr)
    , m_fontPath("")
{}

EC_Text::~EC_Text()
{
    m_pTextManager->RemoveTextComp(this);
}

void EC_Text::Init()
{
    m_pTextManager->AddTextComp(this);
    m_pTransform = m_pParent->AddComponent(new EC_Transform);
}

void EC_Text::Update(float delta)
{}

void EC_Text::SetFont(const char* relativePath, unsigned int size)
{
    m_fontPath = relativePath;
    m_size = size;

    m_pTextManager->UnloadFont(m_font);
    m_font = m_pTextManager->LoadFont(relativePath, size);
}

void EC_Text::SetSize(unsigned int newSize)
{
    if (newSize == m_size) return;

    SetFont(m_fontPath, newSize);
}

void EC_Text::SetColor(Color newColor)
{
    m_color = newColor;
}

Color EC_Text::GetColor()
{
    return m_color;
}

void* EC_Text::GetFontObject()
{
    return m_font;
}

void EC_Text::SetText(const char* text)
{
    m_text = text;
}

const char* EC_Text::GetText()
{
    return m_text.c_str();
}