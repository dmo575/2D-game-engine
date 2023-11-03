#include "SDL_RendererModule.h"
#include "SDL_ResourceModule.h"
#include "RenderableObjec.h"
#include "EC_Transform.h"
#include "EC_Sprite.h"
#include "EC_Text.h"
#include "EC_Shape.h"
#include "SpriteManager.h"
#include "TextManager.h"
#include "ShapeManager.h"
#include "Entity.h"
#include "Logger.h"
#include "MathModule.h"
#include <memory>
#include <math.h>
#include <cstdlib>
#include <SDL_ttf.h>
#include <iostream>

SDL_RendererModule::SDL_RendererModule()
    : m_pWin(nullptr)
    , m_pRen(nullptr)
{}

SDL_RendererModule::~SDL_RendererModule()
{}

bool SDL_RendererModule::Init()
{
    m_pWin = SDL_CreateWindow("Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, 0);

    if (m_pWin == nullptr)
    {
        Logger::Log(Logger::LOG_LEVEL::k_Error, "SDL_RendererModule could not create a Window");
        return false;
    }

    m_pRen = SDL_CreateRenderer(m_pWin, -1, SDL_RENDERER_ACCELERATED);

    if (m_pRen == nullptr)
    {
        Logger::Log(Logger::LOG_LEVEL::k_Error, "SDL_RendererModule could not create a Renderer");
        return false;
    }

    if(m_self == nullptr)
        m_self = this;

    Logger::Log(Logger::LOG_LEVEL::k_Info, "SDL_RendererModule initialized");
    return true;
}

void SDL_RendererModule::DeInit()
{
    Logger::Log(Logger::LOG_LEVEL::k_Info, "SDL_RendererModule de-initialized");
}

void SDL_RendererModule::DrawSprites(SpriteManager* spriteManager)
{
    SDL_SetRenderDrawColor(m_pRen, 125, 125, 125, 255);
    SDL_RenderClear(m_pRen);
    for (EC_Sprite* sprite : *spriteManager->GetSprites())
    {
        if ((sprite->GetTexturePointer() == nullptr && sprite->GetSpriteType() == EC_Sprite::SpriteType::k_sprite) ||
               !sprite->IsActive())
               continue;

        Rect srcRect = sprite->GetSrcRect();
        Rect worldRect = sprite->GetWorldRect();
        SDL_Rect sdl_srcRect{ srcRect.x, srcRect.y, srcRect.w, srcRect.h };
        SDL_Rect sdl_destRect{ worldRect.x, worldRect.y, worldRect.w, worldRect.h };


        if (sprite->GetSpriteType() == EC_Sprite::SpriteType::k_sprite)
        {

            SDL_Texture* texture = (SDL_Texture*)sprite->GetTexturePointer();

            if (!sprite->IsTile())
                SDL_RenderCopy(m_pRen, texture, &sdl_srcRect, &sdl_destRect);
            else
            {
                Point data = sprite->GetTileData();

                for (int y = 0; y < data.y; ++y)
                {
                    for (int x = 0; x < data.x; ++x)
                    {
                        SDL_RenderCopy(m_pRen, texture, &sdl_srcRect, &sdl_destRect);
                        sdl_destRect.x += sdl_destRect.w;
                    }
                    sdl_destRect.y += worldRect.h;
                    sdl_destRect.x = static_cast<int>(worldRect.x);
                }
            }
        }
        else if (sprite->GetSpriteType() == EC_Sprite::SpriteType::k_shapeRect)
        {
            Color c = sprite->GetShapeColor();
            SDL_SetRenderDrawColor(m_pRen, c.r, c.g, c.b, c.a);
            
            if (!sprite->IsTile())
                SDL_RenderFillRect(m_pRen, &sdl_destRect);
            else
            {
                //With shaps, this could be changed so that only one big rect is drawn
                Point data = sprite->GetTileData();

                for (int y = 0; y < data.y; ++y)
                {
                    for (int x = 0; x < data.x; ++x)
                    {
                        SDL_RenderFillRect(m_pRen, &sdl_destRect);
                        sdl_destRect.x += sdl_destRect.w;
                    }
                    sdl_destRect.y += worldRect.h;
                    sdl_destRect.x = static_cast<int>(worldRect.x);
                }
            }
        }
    }
}

void SDL_RendererModule::DrawText(TextManager* textManager)
{
    for (auto t : *textManager->GetTexts())
    {
        if (!t->IsActive()) continue;

        SDL_Rect rect;
        Point fontPos = t->GetParent()->GetComponent<EC_Transform>()->GetPos();
        void* fontPointer = t->GetFontObject();
        const char* text = t->GetText();

        SDL_Color color;
        color.r = t->GetColor().r;
        color.g = t->GetColor().g;
        color.b = t->GetColor().b;
        color.a = t->GetColor().a;

        auto surface = TTF_RenderText_Solid((TTF_Font*)fontPointer, text, color);
        auto texture = SDL_CreateTextureFromSurface(m_pRen, surface);

        rect.x = static_cast<int>(fontPos.x);
        rect.y = static_cast<int>(fontPos.y);
        rect.w = surface->w;
        rect.h = surface->h;

        SDL_FreeSurface(surface);

        SDL_RenderCopy(m_pRen, texture, NULL, &rect);

        SDL_DestroyTexture(texture);
    }
}

void SDL_RendererModule::DrawObjects(const std::vector<RenderableObject*>& objs)
{

    for (RenderableObject* obj : objs)
    {
        if (!obj) continue;

        SDL_SetRenderDrawColor(m_pRen, obj->m_color.r, obj->m_color.g, obj->m_color.b, obj->m_color.a);

        switch (obj->m_type)
        {
        case RenderableObjectType::k_Line:
        {
            SDL_RenderDrawLine(m_pRen, obj->m_data.x, obj->m_data.y, obj->m_data.w, obj->m_data.h);
        }
        break;

        case RenderableObjectType::k_Rect:
        {
            SDL_Rect rect;
            rect.x = obj->m_data.x;
            rect.y = obj->m_data.y;
            rect.w = obj->m_data.w;
            rect.h = obj->m_data.h;

            SDL_RenderDrawRect(m_pRen, &rect);
        }
        break;

        default:
            Logger::Log(Logger::LOG_LEVEL::k_Error, "Trying to draw an undefined \"RenderableObjectType\"");
            break;
        }
    }
}

void SDL_RendererModule::PresentFrame()
{
    SDL_RenderPresent(m_pRen);
}

SDL_Renderer* SDL_RendererModule::GetRenderer()
{
    return m_pRen;
}


void SDL_RendererModule::ClearScreen()
{
    SDL_SetRenderDrawColor(m_pRen, 0, 0, 0, 255);
    SDL_RenderClear(m_pRen);
}

void SDL_RendererModule::DrawImage(void* image, Rect destRect)
{
    SDL_Texture* texture = (SDL_Texture*)image;

    if (!texture)
    {
        Logger::Log(Logger::LOG_LEVEL::k_Error, "SDL_Renderer cannot find texture");
        return;
    }

    SDL_Rect sdl_destRect;
    sdl_destRect.x = destRect.x;
    sdl_destRect.y = destRect.y;
    sdl_destRect.w = destRect.w;
    sdl_destRect.h = destRect.h;

    SDL_RenderCopy(m_pRen, texture, NULL, &sdl_destRect);
    SDL_RenderPresent(m_pRen);
}

void SDL_RendererModule::DrawShapes(ShapeManager* shapeManager)
{
    for (EC_Shape* shape : *shapeManager->GetShapes())
    {
        size_t count;
        auto quads = shape->GetFinalShape(count);

        for (size_t i = 0; i < count; ++i)
        {
            DrawQuad(quads[i]);
        }
    }
}

void SDL_RendererModule::DrawQuad(Quad2D& quad)
{
    SDL_SetRenderDrawColor(m_pRen, 0, 0, 255, 255);

    DrawRectangle(quad.p[0],quad.p[1],quad.p[2], 3);
    DrawRectangle(quad.p[0],quad.p[3],quad.p[2], 3);
}


void SDL_RendererModule::DrawRectangle(Point p1, Point p2, Point p3, float fillRectWidth)
{
    Point leftPoint;
    Point midPoint;
    Point rightPoint;
    
    Point points[3];
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    size_t leftIndex = 0;
    size_t midIndex = 0;
    size_t rightIndex = 0;
    
    for (size_t i = 0; i < 3; ++i)
    {
        if (points[i].x < points[leftIndex].x)
            leftIndex = i;
        if (points[i].x > points[rightIndex].x)
            rightIndex = i;
    }
    
    for (size_t i = 0; i < 3; ++i)
    {
        if (i != leftIndex && i != rightIndex)
        {
            midIndex = i;
            break;
        }
    }
    leftPoint = points[leftIndex];
    midPoint = points[midIndex];
    rightPoint = points[rightIndex];


    Point vecLeftToRight = MathModule::GetDirection(leftPoint, rightPoint);
    //check for nan, set to zero if found
    if (vecLeftToRight.x != vecLeftToRight.x)
        vecLeftToRight.x = 0.0f;
    if (vecLeftToRight.y != vecLeftToRight.y)
        vecLeftToRight.y = 0.0f;

    Point vecLeftToMid = MathModule::GetDirection(leftPoint, midPoint);
    //check for nan
    if (vecLeftToMid.x != vecLeftToMid.x)
        vecLeftToMid.x = 0.0f;
    if (vecLeftToMid.y != vecLeftToMid.y)
        vecLeftToMid.y = 0.0f;

    Point vecMidToRight = MathModule::GetDirection(midPoint, rightPoint);
    //check for nan
    if (vecMidToRight.x != vecMidToRight.x)
        vecMidToRight.x = 0.0f;
    if (vecMidToRight.y != vecMidToRight.y)
        vecMidToRight.y = 0.0f;

    SDL_Rect rect;
    float upperCounter = 0.0f;
    //Bool that keeps track of when to change the lowerVec from the LeftToLow to the LowToUp
    bool changeVec = false;

    //The var fillRectWidth will have a tremendous impact on how many times this while runs
    while (true/*upperCounter < 300.0f*/)
    {
        /*ABOUT:
        * TracePoints are the current point in a line.
        * -longerTracePoint marks where the current point in the longest of the three lines of the
        * rectangle is at the moment.
        * -shortestTracePoint marks where the current point in the CURRENT short line of the two we
        * are using is located at the moment.
        */

        //This tracer tracks a point that travels from the most left point in the triangle
        //to the most right point of the triangle.
        Point longerTracePoint = leftPoint + (vecLeftToRight * fillRectWidth * upperCounter);
        //This tracer keeps track of a point that travels from the most left point of the triangle
        //to the middle point in the triangle and then from the middle point in the triangle to the
        //most right point in the triangle
        Point shortestTracePoint;

        //This point keeps track of the directional vector that the shortest tracer must use
        //It makes sure to update itself if the tracer changes from one point to another so it
        //can provide the correct direction
        Point shortestDirVec;

        //If the shortest tracer has NOT reached the middle point. It means its moving towards it
        if (!changeVec)
        {
            shortestTracePoint = leftPoint;
            shortestDirVec = vecLeftToMid;
        }//If the shortest tracer HAS ALREADY reached the middle point. It means its moving towards the most right point
        else
        {
            shortestTracePoint = midPoint;
            shortestDirVec = vecMidToRight;
        }


        //This while loop is used to help properly pinpoint the shortestTracerPoint location
        //Because we use the longestTracePoint as the one that decides the horizontal amount to move
        //each loop, the shortestTracerPoint must also move that ammount.
        //This loop usually runs no more than 5 times
        int safe = 50;
        float length = 100.0f;
        float dir = 1.0f;
        Point temp = shortestTracePoint;//STP is just the origin point at this time
        while (std::fabs(temp.x - longerTracePoint.x) > 0.5f && safe > 0)//While dif > 1.0f
        {
            //Add
            temp = temp + shortestDirVec * length * dir;
            //evaluate
            if (temp.x > longerTracePoint.x && dir > 0.0f)
            {
                //update
                length /= 2.0f;
                dir = -1.0f;
            }
            else if (temp.x < longerTracePoint.x && dir < 0.0f)
            {
                //update
                length /= 2.0f;
                dir = 1.0f;
            }
            safe--;
        }

        shortestTracePoint = temp;

        //Now that we have the two points, we just need to create the rect.

        //This checks which point is on top and uses its data to get the rect's X and Y
        //The point at the bottom is used to get the height only.
        if (leftPoint.y < midPoint.y)
        {
            rect.x = static_cast<int>(longerTracePoint.x);
            rect.y = static_cast<int>(longerTracePoint.y);
            rect.w = static_cast<int>(fillRectWidth);
            rect.h = static_cast<int>(std::fabs(longerTracePoint.y - shortestTracePoint.y));
        }
        else
        {
            rect.x = static_cast<int>(shortestTracePoint.x);
            rect.y = static_cast<int>(shortestTracePoint.y);
            rect.w = static_cast<int>(fillRectWidth);
            rect.h = static_cast<int>(std::fabs(shortestTracePoint.y - longerTracePoint.y));
        }

        //If the longestTracerPoint has already passed the mid point, we must update
        if (longerTracePoint.x > midPoint.x && !changeVec)
        {
            changeVec = true;
            continue;
        }//Once we are past the most right point, we can return
        else if (longerTracePoint.x >= rightPoint.x)
            break;

        SDL_RenderFillRect(m_pRen, &rect);
        upperCounter++;
    }
}