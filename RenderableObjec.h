#pragma once
#include "CoreStructs.h"//      Rect, Color

//NOTE: This whole thing should be inside CoreStructs I think.

enum class RenderableObjectType { k_Line, k_Rect };

struct RenderableObject
{
public:
    RenderableObject(RenderableObjectType type, Rect data, Color color)
        : m_type(type)
        , m_data(data)
        , m_color(color)
    {}

public:
    RenderableObjectType m_type;
    Rect m_data;
    Color m_color;
};