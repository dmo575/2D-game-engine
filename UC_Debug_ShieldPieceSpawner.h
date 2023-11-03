#pragma once
#include "Component.h"
#include <vector>
#include <string>

//Spawn the shields with a collider each.

class UC_Debug_ShieldPieceSpawner : public Component
{
public:
    UC_Debug_ShieldPieceSpawner();
    virtual ~UC_Debug_ShieldPieceSpawner() override;

private:
    virtual void Init() override;


private:
    std::vector<Point> m_shieldPiecesPos;
    float m_shieldPX;
    float m_shieldPY;
    std::string m_shieldImg;
    Rect m_srcRect;
};