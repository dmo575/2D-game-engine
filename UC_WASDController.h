#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Point, KeyCode

class EC_Transform;

class UC_WASDController : public Component
{
public:
    enum class DirType {EIGHT, FOUR};

public:
    UC_WASDController(float vel, DirType dir);
    virtual ~UC_WASDController() override;

    void SetVel(float newVel);
    Point GetCurrDir();
    float GetVel();
    void ApplyMovement(bool value);

private:
    virtual void Update(float delta) override;
    virtual void Init() override;

    void UpdateWASD8(float delta);
    void UpdateWASD4(float delta);
    void AddToKeysOrder(KeyCode key);
    void RemoveFromKeysOrder(KeyCode key);
    void RemoveSpaceFromKeysOrder(int spaceIndex);
    KeyCode GetLastestKey();

private:
    KeyCode m_keysOrder[4];
    EC_Transform* m_pTransform;
    float m_vel;
    DirType m_dirType;
    Point m_currDir;
    bool m_applyMov;
};