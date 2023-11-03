#pragma once
#include "Component.h"
#include "CoreStructs.h"
#include <string>

class EC_Transform;
class TextManager;
class EC_Text : public Component
{
    friend class TextManager;
public:
    EC_Text();
    virtual ~EC_Text() override;

    void SetFont(const char* relativePath, unsigned int size);
    void SetSize(unsigned int newSize);
    void SetColor(Color newColor);
    Color GetColor();
    void* GetFontObject();
    void SetText(const char* text);
    const char* GetText();

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    static TextManager* m_pTextManager;
    EC_Transform* m_pTransform;
    std::string m_text;
    Color m_color;
    unsigned int m_size;
    const char* m_fontPath;
    void* m_font;
};