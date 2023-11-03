#pragma once
#include <vector>//         std::vector
#include <string>//         std::string

class EC_Sprite;

class SpriteManager
{
private:
    //Internal struct that keeps data about a loaded texture
    struct TxData
    {
    public:
        TxData(std::string name)
            : m_name(name)
            , m_references(1)
        {}

    public:
        std::string m_name;
        int m_references;
    };


    using pair_txData_pointer = std::pair <TxData, void*>;
    using pair_layer_lastIndex = std::pair <unsigned int, size_t>;


public:
    SpriteManager();
    ~SpriteManager();

    void AddSpriteComp(EC_Sprite* toAdd);
    void RemoveSprite(EC_Sprite* toRemove);
    void MoveSpriteToLayer(EC_Sprite* sprite, unsigned int layer, bool atTheTop = true);
    const std::vector<EC_Sprite*>* GetSprites() const;

    void* LoadTextureFromName(std::string newTx, std::string& oldTxRef);
    void UnloadTextureFromIndex(size_t index);

    //----------------------------------------------------------------- DEBUG - DEBUG - DEBUG
    void DEBUG_CollecTexturetData();
    void DEBUG_CollecSpritetData();
    std::string DEBUG_textureData;
    std::string DEBUG_spriteData;

private:
    void Init();

private:
    std::vector<EC_Sprite*> m_spriteComps;
    std::vector<pair_txData_pointer> m_textures;
    std::vector<pair_layer_lastIndex> m_layerToLastIndex;
};