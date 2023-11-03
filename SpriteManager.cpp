#include "SpriteManager.h"
#include "ResourceModule.h"
#include "EC_Sprite.h"
#include <iostream>

//Needed for DEBUG
#include "Entity.h"

SpriteManager::SpriteManager()
{
    Init();
}

SpriteManager::~SpriteManager()
{
    for (size_t i = 0; i < m_textures.size(); ++i)
        UnloadTextureFromIndex(i);
}

void SpriteManager::Init()
{
    EC_Sprite::m_pSpriteManager = this;
}

void SpriteManager::AddSpriteComp(EC_Sprite* toAdd)
{
    EC_Sprite* temp = toAdd;

    if (m_spriteComps.size() > 0)
    {
        for (size_t i = static_cast<size_t>(0); i <= m_spriteComps.size() -static_cast<size_t>(1); ++i)
        {
            if (m_spriteComps[i]->GetLayer() > toAdd->GetLayer())
            {
                temp = m_spriteComps[i];
                m_spriteComps[i] = toAdd;
                toAdd = temp;
            }
        }
    }

    m_spriteComps.push_back(toAdd);
}

void SpriteManager::RemoveSprite(EC_Sprite* toRemove)
{
    //Updates texture vector and handles texture if its ref count reaches 0
    for (size_t i = static_cast<size_t>(0); i < m_textures.size(); ++i)
    {
        if (m_textures[i].first.m_name == toRemove->m_textureName)
        {
            m_textures[i].first.m_references--;
            if (m_textures[i].first.m_references == static_cast<size_t>(0))
            {
                UnloadTextureFromIndex(i);
                m_textures.erase(m_textures.begin() + i);
            }
            break;
        }
    }

    //updates sprites vector
    for (size_t i = static_cast<size_t>(0); i < m_spriteComps.size(); ++i)
    {
        if (toRemove == m_spriteComps[i])
        {
            m_spriteComps.erase(m_spriteComps.begin() + i);
            return;
        }
    }
}

void SpriteManager::MoveSpriteToLayer(EC_Sprite* sprite, unsigned int layer, bool atTheTop)
{
    //Find the sprite in the sprites vector and remove it from the vector
    for (size_t i = 0; i < m_spriteComps.size(); ++i)
    {
        if (sprite == m_spriteComps[i])
        {
            m_spriteComps.erase(m_spriteComps.begin() + i);
            break;
        }
    }

    //Find 
    if (!atTheTop)
    {
        for (size_t i = 0; i < m_spriteComps.size(); ++i)
        {
            if (sprite->GetLayer() <= m_spriteComps[i]->GetLayer())
            {
                m_spriteComps.insert(m_spriteComps.begin() + i, sprite);
                return;
            }
        }

        m_spriteComps.push_back(sprite);
    }
    else
    {
        size_t spriteCount = m_spriteComps.size() == 0 ? 0 : m_spriteComps.size();

        for (size_t i = spriteCount; i > 0; --i)
        {
            if (sprite->GetLayer() >= m_spriteComps[i - 1]->GetLayer())
            {
                m_spriteComps.insert(m_spriteComps.begin() + i, sprite);
                return;
            }
        }

        m_spriteComps.push_back(sprite);
    }
}


//Returns a void pointer to the loaded texture
void* SpriteManager::LoadTextureFromName(std::string newTx, std::string& oldTxRef)
{

    //Get the indexes of both the texture we want and the one we are using.
    //Returns -1 in any of the cases it doesnt find the texture for.
    int textureNameIndex = -1;
    int currTextureNameIndex = -1;

    for (size_t i = 0; i < m_textures.size(); ++i)
    {
        if (m_textures[i].first.m_name == newTx)
        {
            textureNameIndex = static_cast<int>(i);
            if (currTextureNameIndex != -1)
                break;
        }


        if (m_textures[i].first.m_name == oldTxRef)
        {
            currTextureNameIndex = static_cast<int>(i);
            if (textureNameIndex != -1)
                break;
        }
    }

    //If we are asking for the same texture we already have, just return it again.
    if (oldTxRef == newTx)
        return m_textures[currTextureNameIndex].second;


    //If we were using a texture before this, update refCount. Unload if needed.
    //HANDLES:
    //-OLD TX REF COUNT YES
    //-OLD TX MEM SPACE YES
    //-OLD TX VEC SPACE NO
    if (currTextureNameIndex > -1)
    {
        m_textures[currTextureNameIndex].first.m_references--;

        if(m_textures[currTextureNameIndex].first.m_references < 1)
            UnloadTextureFromIndex(currTextureNameIndex);
    }

    //Will storage a pointer to our desired texture.
    void* newTexture = nullptr;

    //If the texture we want its already loaded. Get a pointer, updte ref count.
    if (textureNameIndex > -1)
    {
        m_textures[textureNameIndex].first.m_references++;
        newTexture = m_textures[textureNameIndex].second;
    }
    else//Wanted texture not loaded, load it, get pointer, update refCount (auto)
    {
        std::string path = ResourceModule::Get()->GetBasePath() + std::string("Resources/") + newTx;
        newTexture = ResourceModule::Get()->LoadTexture(path.c_str());
        m_textures.push_back(pair_txData_pointer(TxData(newTx), newTexture));
    }

    //Ensures to erase the old texture from the array if its refCount is 0.
    if (currTextureNameIndex > -1)
    {
        if (m_textures[currTextureNameIndex].first.m_references == 0)
            m_textures.erase(m_textures.begin() + currTextureNameIndex);
    }

    //Updates the Sprite
    oldTxRef = newTx;
    return newTexture;
}

//Unloads the texture from memory
void SpriteManager::UnloadTextureFromIndex(size_t index)
{
    ResourceModule::Get()->UnloadTexture(m_textures[index].second);
}

const std::vector<EC_Sprite*>* SpriteManager::GetSprites() const
{
    return &m_spriteComps;
}

void SpriteManager::DEBUG_CollecTexturetData()
{
    DEBUG_textureData = "";
    DEBUG_textureData += "----------------------------------------------------------LOADED_TEXTURES_DATA\n";

    DEBUG_textureData += "   TEXTURE'S NAME      |      REF. COUNT\n";


    size_t count = static_cast<size_t>(0);
    size_t index = static_cast<size_t>(0);
    for (pair_txData_pointer t : m_textures)
    {
        count = t.first.m_name.length();
        DEBUG_textureData += std::to_string(index);
        DEBUG_textureData += ("_ " + t.first.m_name);
    
        while (count < static_cast<size_t>(22))
        {
            DEBUG_textureData += "-";
            count++;
        }
        DEBUG_textureData += "----   (" + std::to_string(t.first.m_references) + std::string(")\n");
    
        index++;
    }
}

void SpriteManager::DEBUG_CollecSpritetData()
{

    DEBUG_spriteData = "";
    DEBUG_spriteData += "------------------------------------------------------------------SPRITES_DATA\n";
    DEBUG_spriteData += "   ENTITY       |        SRC_TX         |       SRC_RECT\n";

    size_t count = static_cast<size_t>(0);
    size_t index = static_cast<size_t>(0);
    size_t diff = static_cast<size_t>(0);
    for (auto t : m_spriteComps)
    {
        count = t->m_pParent->m_name.length();
        DEBUG_spriteData += std::to_string(index);
        DEBUG_spriteData += "_ " + t->m_pParent->m_name;
    
        diff = index / static_cast<size_t>(10);
        while (count < static_cast<size_t>(15) - diff)
        {
            DEBUG_spriteData += " ";
            count++;
        }
        count = t->m_textureName.length();
        DEBUG_spriteData+=  t->m_textureName;
    
        while (count < static_cast<size_t>(25))
        {
            DEBUG_spriteData += "-";
            count++;
        }
        Rect r = t->GetSrcRect();
        DEBUG_spriteData += "   (" +
            std::to_string(r.x) + 
            ", " + std::to_string(r.y) + 
            ", " + std::to_string (r.w) + 
            ", " + std::to_string(r.h) + ")" + 
            "(L" + std::to_string(t->GetLayer()) + ")\n";


    
        index++;
    }
}