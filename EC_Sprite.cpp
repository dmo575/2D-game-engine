#include <cstring>
#include "EC_Sprite.h"
#include "EC_Includes.h"
#include "SpriteManager.h"
#include "EC_Transform.h"

SpriteManager* EC_Sprite::m_pSpriteManager = nullptr;

EC_Sprite::EC_Sprite(const char* textureToAdd, unsigned int layer)
    : Component()
    , m_pTransform(nullptr)
    , m_pTexturePointer(nullptr)
    , m_textureName("NULL")
    , m_srcRect(Rect(0, 0, 64, 64))
    , m_pivot(0, 0)
    , m_tile(false)
    , m_tileData()
    , m_layer(layer)
    , m_shape(BasicShape::k_Circle)
    , m_shapeColor(Color::PreDefined::k_White)
    , m_type(SpriteType::k_sprite)
{
    m_pSpriteManager->AddSpriteComp(this);

    if (strcmp(textureToAdd, "") != 0)
        SetSourceImg(textureToAdd, m_srcRect);
}

EC_Sprite::EC_Sprite(unsigned int layer)
    : EC_Sprite("", layer)
{}

EC_Sprite::EC_Sprite(const char* textureToAdd, Rect& srcRect, unsigned int layer)
    : Component()
    , m_pTransform(nullptr)
    , m_pTexturePointer(nullptr)
    , m_textureName("NULL")
    , m_srcRect(srcRect)
    , m_pivot(0, 0)
    , m_tile(false)
    , m_tileData()
    , m_layer(layer)
    , m_shape(BasicShape::k_Circle)
    , m_shapeColor(Color::PreDefined::k_White)
    , m_type(SpriteType::k_sprite)
{
    m_pSpriteManager->AddSpriteComp(this);

    if (strcmp(textureToAdd, "") != 0)
        SetSourceImg(textureToAdd, m_srcRect);
}

EC_Sprite::~EC_Sprite()
{
    m_pSpriteManager->RemoveSprite(this);
}

void EC_Sprite::Init()
{
    m_pTransform = m_pParent->GetComponent<EC_Transform>();

    if (!m_pTransform)
    {
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("Missing EC_Transform for EC_Sprite in \"" + m_pParent->GetName() + std::string("\"")).c_str());
        return;
    }

    SetLayer(m_layer);
}

void EC_Sprite::Update(float delta)
{}

//textureName: The texture's filename to use
//rect: the src rect
void EC_Sprite::SetSourceImg(const char* textureName, int xSrc, int ySrc, int width, int height)
{
    m_pTexturePointer = m_pSpriteManager->LoadTextureFromName(textureName, m_textureName);

    m_srcRect.x = xSrc;
    m_srcRect.y = ySrc;
    m_srcRect.w = width;
    m_srcRect.h = height;

    m_type = SpriteType::k_sprite;
}

//textureName: The texture's filename to use
//rect: the src rect
void EC_Sprite::SetSourceImg(const char* textureName, Rect& rect)
{
    SetSourceImg(textureName, rect.x, rect.y, rect.w, rect.h);
}

//Returns the texture's filename
const char* EC_Sprite::GetTextureName()
{
    return m_textureName.c_str();
}

//Returns a void* to the texture
const void* EC_Sprite::GetTexturePointer() const
{
    return m_pTexturePointer;
}

const Rect EC_Sprite::GetSrcRect() const
{
    return m_srcRect;
}

const Rect EC_Sprite::GetWorldRect() const
{
    Rect temp;

    //This leaves temp with the correct X and W WORLD COORDS after applying
    //the pivot offset.
    temp.x = static_cast<int>((m_pivot.x * m_pTransform->GetScale().x) + m_pTransform->GetPos().x);
    temp.y = static_cast<int>((m_pivot.y * m_pTransform->GetScale().y) + m_pTransform->GetPos().y);

    //This applies the correct width and height, taking scale into consideration.
    temp.w = static_cast<int>(m_srcRect.w * m_pTransform->GetScale().x);
    temp.h = static_cast<int>(m_srcRect.h * m_pTransform->GetScale().y);

    return temp;
}

//If true: x and Y will decide how many times to tile it in each axis
//If false: sprite will be set to not tile
void EC_Sprite::SetTile(bool value, int x, int y)
{
    m_tile = value;
    m_tileData.x = static_cast<float>(x);
    m_tileData.y = static_cast<float>(y);
}

//Returns if its set to  tile or not
bool EC_Sprite::IsTile()
{
    return m_tile;
}

//Returns the tile data: The ammount of times it has to tile in both X and Y axis
Point EC_Sprite::GetTileData()
{
    return m_tileData;
}

//Set the pivot of the sprite, Transform's pos is the origin
void EC_Sprite::SetPivot(Point pivot)
{
    m_pivot = pivot;
}

//Set the pivot so that the sprite is centered to the transform's position
void EC_Sprite::SetPivotToCenter()
{
    m_pivot.x = static_cast<float>(-(m_srcRect.w / 2));
    m_pivot.y = static_cast<float>(-(m_srcRect.h / 2));
}

void EC_Sprite::SetSourceRect(Rect& rect)
{
    SetSourceRect(rect.x, rect.y, rect.w, rect.h);
}

void EC_Sprite::SetSourceRect(int x, int y, int w, int h)
{
    m_srcRect.x = x;
    m_srcRect.y = y;
    m_srcRect.w = w;
    m_srcRect.h = h;
}

void EC_Sprite::SetLayer(unsigned int value, bool atTheTop)
{
    //If new layer is different than current layer, call SpriteManager::MoveSpriteToLayer(sprite, old layer, new layer);
    m_layer = value;
    m_pSpriteManager->MoveSpriteToLayer(this, value, atTheTop);
}

unsigned int EC_Sprite::GetLayer()
{
    return m_layer;
}

void EC_Sprite::SetBasicShape_Rect(Rect rectData, Color circleColor)
{
    m_srcRect = rectData;
    m_shapeColor = circleColor;
    m_type = SpriteType::k_shapeRect;
}

Color EC_Sprite::GetShapeColor()
{
    return m_shapeColor;
}

EC_Sprite::SpriteType EC_Sprite::GetSpriteType()
{
    return m_type;
}