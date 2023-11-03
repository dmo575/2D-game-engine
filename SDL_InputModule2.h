#pragma once
#include "InputModule.h"
#include <vector>
#include "SDL.h"

class SDL_InputModule2 : public InputModule
{
    friend class Engine;

private:
    struct KeyData
    {
        KeyData()
            : m_name("UNKNOWN")
            , m_state(KeyState::k_KS_Null)
            , m_keyCode(KeyCode::k_Unknown)
        {};

        const char* m_name;
        KeyState m_state;
        KeyCode m_keyCode;
    };

public:
    virtual KeyState GetKeyState(KeyCode key) const override;
    virtual bool IsAnyKeyPressed(KeyState pressType) override;
    virtual Point GetMousePos() override;

private:
    SDL_InputModule2();
    virtual ~SDL_InputModule2() override;

    virtual bool Init() override;
    virtual void Update() override;
    virtual void DeInit() override;

    void UpdateKeyState(KeyCode key, KeyState newState);
    void UpdateCurrentKeys();
    KeyCode GetKeyCode(SDL_KeyCode key);
    const char* GetName(KeyCode key);

private:
    std::vector<KeyData> m_keysData;
    Point m_mousePos;
};