#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Point, Rect
#include <string>//             std::string

/*
* SOURCE IMAGE: Is the image we load to get a texture from
*
* TEXTURE: Is the texture we use to get sprites from
*
* SOURCE RECT: Is the rect that defines the sprite within the texture.
*   --The top left corner of the texture is 0,0
*
* PIVOT: Is the offset applied to the Transform for when drawing the sprite.
*   --We assume that drawing starts at top left corner + the offset. We draw
*     Assuming the transform is the 0.0.
*
* WORLD RECT: Is the final rect in world space.
*/

class Entity;
class EC_Transform;
class SpriteManager;

class EC_Sprite : public Component
{
    friend class SpriteManager;

public:
    enum class SpriteType {k_sprite, k_shapeRect};

public:
    EC_Sprite(unsigned int layer = 0U);
    EC_Sprite(const char* textureToAdd, unsigned int layer = 0U);
    EC_Sprite(const char* textureToAdd, Rect& srcRect, unsigned int layer = 0U);
    virtual ~EC_Sprite() override;


    //------------------------------------------------------ SOURCE IMAGE
    void SetSourceImg(const char* textureName, int srcX, int srcY, int width, int height);
    void SetSourceImg(const char* textureName, Rect& rect);

    //------------------------------------------------------ SHAPES
    void SetBasicShape_Rect(Rect rectData, Color rectColor);
    Color GetShapeColor();
    SpriteType GetSpriteType();

    //------------------------------------------------------ TEXTURE
    const char* GetTextureName();
    const void* GetTexturePointer() const;

    //------------------------------------------------------ SOURCE RECT
    const Rect GetSrcRect() const;
    void SetSourceRect(Rect& rect);
    void SetSourceRect(int x, int y, int w, int h);

    //------------------------------------------------------ WORLD RECT
    const Rect GetWorldRect() const;

    //------------------------------------------------------ PIVOT
    void SetPivot(Point offset);
    void SetPivotToCenter();

    //------------------------------------------------------ TILE
    void SetTile(bool value, int x = 0, int y = 0);
    bool IsTile();
    Point GetTileData();

    //------------------------------------------------------ LAYER
    void SetLayer(unsigned int value, bool atTheTop = true);
    unsigned int GetLayer();
private:
    virtual void Init() override;
    virtual void Update(float delta) override;


private:
    static SpriteManager* m_pSpriteManager;
    EC_Transform* m_pTransform;
    void* m_pTexturePointer;
    std::string m_textureName;

    Rect m_srcRect;
    Point m_pivot;

    Point m_tileData;
    bool m_tile;
    unsigned int m_layer;

    SpriteType m_type;
    BasicShape m_shape;
    Color m_shapeColor;
};