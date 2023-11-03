#pragma once
#include "RendererModule.h"
#include "SDL.h"

class SDL_RendererModule : public RendererModule
{
    friend class Engine;
    friend class SDL_ResourceModule;

private:
    SDL_RendererModule();
    virtual ~SDL_RendererModule() override;

    virtual bool Init() override;
    virtual void DeInit() override;

    virtual void DrawSprites(SpriteManager* spriteManager) override;
    virtual void DrawText(TextManager* textManager) override;
    virtual void DrawObjects(const std::vector<RenderableObject*>& objs) override;
    virtual void PresentFrame() override;
    virtual void ClearScreen() override;
    virtual void DrawImage(void* image, Rect destRect) override;
    virtual void DrawShapes(ShapeManager* shapeManager) override;


    void DrawQuad(Quad2D& quad);
    void DrawRectangle(Point leftPoint, Point lowerPoint, Point upperPoint, float fillRectWidth);

private:
    SDL_Renderer* GetRenderer();

private:
    SDL_Window* m_pWin;
    SDL_Renderer* m_pRen;
};