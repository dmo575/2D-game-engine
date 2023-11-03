#include "UC_Debug_ShieldPieceSpawner.h"
#include "UC_Includes.h"

UC_Debug_ShieldPieceSpawner::UC_Debug_ShieldPieceSpawner()
    : m_shieldPX((64 / 2) / 2)
    , m_shieldPY((102 - (96 / 3)) / 2)
    , m_shieldImg("Shooter/SP_Shield.png")
    , m_srcRect(0, 0, 64, 96)
{}

UC_Debug_ShieldPieceSpawner::~UC_Debug_ShieldPieceSpawner()
{}

void UC_Debug_ShieldPieceSpawner::Init()
{
    m_shieldPiecesPos.reserve(19);
    m_shieldPiecesPos.push_back(Point{256,256});
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[0] + Point{ m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[1] + Point{ +m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[2] + Point{ -m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[3] + Point{ -(m_shieldPX + 32 / 2), 0.0f });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[4] + Point{ -m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[5] + Point{ +m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[6] + Point{ +m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[7] + Point{ +(m_shieldPX + 32 / 2), 0.0f });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[8] + Point{ +m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[9] + Point{ +m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[10] + Point{ -m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[11] + Point{ -m_shieldPX, +m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[12] + Point{ -(m_shieldPX + 32 / 2), 0.0f });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[13] + Point{ -(m_shieldPX + 32 / 2), 0.0f });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[14] + Point{ -m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[15] + Point{ -m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[16] + Point{ +m_shieldPX, -m_shieldPY });
    m_shieldPiecesPos.push_back(m_shieldPiecesPos[17] + Point{ +m_shieldPX, -m_shieldPY });
  
    Point temp = Point{ 257,257 };


    for (size_t i = 0; i < 19; ++i)
    {
        auto shieldEnt = m_entityManager->AddEntity("SHIELD_PIECE_ENT");

        Point spawnPos = MathModule::GetDirection(temp, m_shieldPiecesPos[i]);

        auto transform = shieldEnt->AddComponent(new EC_Transform);
        transform->SetScale(0.5f, 0.5f);
        transform->SetPos(m_shieldPiecesPos[i]);
        auto sprite = shieldEnt->AddComponent(new EC_Sprite);
        auto collider = shieldEnt->AddComponent(new EC_Collider);
        collider->AttachToSprite(true);
        collider->SetTrigger(true);

        sprite->SetSourceImg(m_shieldImg.c_str(), m_srcRect);
        sprite->SetPivotToCenter();
    }
}