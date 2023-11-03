#include "Component.h"
#include "CoreStructs.h"
#include "IShooterHittable.h"
#include "EventSystem.h"
#include <string>
#include <memory>

class EC_Transform;
class EC_Sprite;


class UC_Demo7_ShieldPiece : public Component, public IEventSender
{
public:
    enum class ShieldPieceState{k_Idle, k_OOB, k_Moving, k_Destroyed};
    //Idle: can take dmg, stays put
    //Init: cannot take dmg, moves to loc
    //Moving: cannot take dmg moves to loc (dif sprite than Init)
public:
    UC_Demo7_ShieldPiece(Point spawnPos);
    virtual ~UC_Demo7_ShieldPiece() override;
    void MoveShieldTo(Point newPos);
    void DamagePiece();
    void GetPieceOut();
    ShieldPieceState GetState();
    virtual void Sub(Event e, IEventListener* listener) override;
    virtual void Unsub(Event e, IEventListener* listener) override;

private:
    virtual void Init() override;
    virtual void Update(float delta) override;
    void SetState(ShieldPieceState newState);
    void UpdateCurrentState(float delta);

private:
    std::unique_ptr<OEventSender> m_pEventSender;
    EC_Transform* m_pTransform;
    EC_Sprite* m_pSprite;
    std::string m_shieldImg;
    Rect m_srcRect;
    ShieldPieceState m_currState;
    Point m_spawnPos;
    Point m_destPos;
    Point m_travelStartPos;
    float m_travelMaxDistance;
    float m_vel;
    unsigned int m_HP;
};