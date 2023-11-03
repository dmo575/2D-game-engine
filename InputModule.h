#pragma once
#include "CoreStructs.h"

struct KeyData
{
public:
    KeyData()
    : m_pKeyName("UNKNOWN")
    , m_state(KeyState::k_KS_Null)
    , m_keyCode(KeyCode::k_Unknown)
    {}

public:
    const char* m_pKeyName;
    KeyState m_state;
    KeyCode m_keyCode;
};

class InputModule
{
    friend class Engine;

public:
    static InputModule* Get()
    {
        return m_self;
    }

    virtual KeyState GetKeyState(KeyCode key) const = 0;
    virtual bool IsAnyKeyPressed(KeyState pressType) = 0;
    virtual Point GetMousePos() = 0;

protected:
    virtual bool Init() = 0;
    virtual void Update() = 0;
    virtual void DeInit() = 0;

protected:
    InputModule() {};
    virtual ~InputModule() {};

protected:
    static InputModule* m_self;
};