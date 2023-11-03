#pragma once
#include "InputModule.h"
#include "SDL.h"

class SDL_InputModule : public InputModule
{
    friend class Engine;

public:
    virtual KeyState GetKeyState(KeyCode key) const override;
    virtual bool IsAnyKeyPressed(KeyState pressType) override;

    virtual Point GetMousePos() override;

private:
    SDL_InputModule();
    virtual ~SDL_InputModule() override;

    virtual bool Init() override;
    virtual void Update() override;
    virtual void DeInit() override;

    int FindKeyInList(KeyCode keyCode);
    void SetKeyState(int listIndex, KeyState newState);
    void ClearKeysWithState(KeyState stateToKill);
    void AddKeyToList(const char* keyName, KeyState state, KeyCode keyCode);

    const char* SDLKeyCodeToChar(SDL_KeyCode keyCode);
    KeyCode SDLKeyCodeToKeyCode(SDL_KeyCode keyCode);

    void DEBUG_PrintArray();

private:
    KeyData m_keyList[64];
    Point m_mousePos;
};