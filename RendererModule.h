#pragma once
#include <vector>
#include "CoreStructs.h"

class EC_Sprite;
class SpriteManager;
class TextManager;
struct RenderableObject;
class ShapeManager;

class RendererModule
{
    friend class Engine;
    friend class LevelManager;

protected:
    RendererModule() {};
    virtual ~RendererModule() {};

    static RendererModule* Get()
    {
        return m_self;
    }

    virtual bool Init() = 0;
    virtual void DeInit() = 0;


    //Tell the renderer to draw all the sprites provided
    virtual void DrawSprites(SpriteManager* spriteManager) = 0;
    
    //Tell the renderer to draw all the texts provided
    virtual void DrawText(TextManager* textManager) = 0;

    //Tell the renderer to draw all the objects given
    virtual void DrawObjects(const std::vector<RenderableObject*>& objs) = 0;

    //Tell the renderer to present everything it got into the screen
    virtual void PresentFrame() = 0;

    //Clears the screen, painting it pitch black
    virtual void ClearScreen() = 0;

    //Draws the entire image onto the desired place
    virtual void DrawImage(void* image, Rect destRect) = 0;

    //Tells the renderer to draw all the shapes provided
    virtual void DrawShapes(ShapeManager* shapeManager) = 0;

protected:
    static RendererModule* m_self;
};