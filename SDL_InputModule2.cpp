#include "SDL_InputModule2.h"
#include "Logger.h"
#include <iostream>

SDL_InputModule2::SDL_InputModule2()
{}

SDL_InputModule2::~SDL_InputModule2()
{}

bool SDL_InputModule2::Init()
{
    if (m_self == nullptr)
        m_self = this;

    Logger::Log(Logger::LOG_LEVEL::k_Info, "SDL_InputModule2 initialized");
    return true;
}

void SDL_InputModule2::DeInit()
{
    Logger::Log(Logger::LOG_LEVEL::k_Info, "SDL_InputModule2 de-initialized");
}

void SDL_InputModule2::Update()
{
    static SDL_Event event;

    UpdateCurrentKeys();

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EventType::SDL_KEYDOWN:
        {
            KeyCode key = GetKeyCode((SDL_KeyCode)event.key.keysym.sym);

            if (key != KeyCode::k_Unknown)
                UpdateKeyState(key, KeyState::k_KS_Press);
        }
            break;
        case SDL_EventType::SDL_KEYUP:
        {
            KeyCode key = GetKeyCode((SDL_KeyCode)event.key.keysym.sym);

            if (key != KeyCode::k_Unknown)
                UpdateKeyState(key, KeyState::k_KS_Release);
        }
            break;

        case SDL_MOUSEMOTION:
        {
            m_mousePos.x = static_cast<float>(event.motion.x);
            m_mousePos.y = static_cast<float>(event.motion.y);
        }
            break;

        case SDL_MOUSEBUTTONDOWN:
        {
            auto mouseButton = (SDL_MouseButtonEvent)event.button;

            if (mouseButton.button == SDL_BUTTON_LEFT)
            {
                UpdateKeyState(KeyCode::k_LMB, KeyState::k_KS_Press);
            }
            else if (mouseButton.button == SDL_BUTTON_MIDDLE)
            {
                UpdateKeyState(KeyCode::k_MMB, KeyState::k_KS_Press);
            }
            else if (mouseButton.button == SDL_BUTTON_RIGHT)
            {
                UpdateKeyState(KeyCode::k_RMB, KeyState::k_KS_Press);
            }
        }
            break;

        case SDL_MOUSEBUTTONUP:
        {
            auto mouseButton = (SDL_MouseButtonEvent)event.button;

            if (mouseButton.button == SDL_BUTTON_LEFT)
            {
                UpdateKeyState(KeyCode::k_LMB, KeyState::k_KS_Release);
            }
            else if (mouseButton.button == SDL_BUTTON_MIDDLE)
            {
                UpdateKeyState(KeyCode::k_MMB, KeyState::k_KS_Release);
            }
            else if (mouseButton.button == SDL_BUTTON_RIGHT)
            {
                UpdateKeyState(KeyCode::k_RMB, KeyState::k_KS_Release);
            }
        }
        break;

        case SDL_WINDOWEVENT:
        {
            if(event.window.event == SDL_WINDOWEVENT_CLOSE)
                UpdateKeyState(KeyCode::k_Win_Close, KeyState::k_KS_Press);
        }
        break;
        }
    }
}

KeyState SDL_InputModule2::GetKeyState(KeyCode key) const
{
    for (size_t i = 0; i < m_keysData.size(); ++i)
    {
        if (m_keysData[i].m_keyCode == key)
            return m_keysData[i].m_state;
    }

    return KeyState::k_KS_Null;
}

bool SDL_InputModule2::IsAnyKeyPressed(KeyState pressType)
{
    for (size_t i = 0; i < m_keysData.size(); ++i)
    {
        if (m_keysData[i].m_state == pressType)
            return true;
    }

    return false;
}

Point SDL_InputModule2::GetMousePos()
{
    return m_mousePos;
}

//Updates the state of a key based on event info
void SDL_InputModule2::UpdateKeyState(KeyCode keyCode, KeyState newState)
{
    size_t keyIndex = static_cast<size_t>(-1);

    for (size_t i = 0; i < m_keysData.size(); ++i)
    {
        if (m_keysData[i].m_keyCode == keyCode)
        {
            keyIndex = i;
        }
        else if (m_keysData[i].m_keyCode== KeyCode::k_Unknown)
        {
            m_keysData.erase(m_keysData.begin() + i);
            i--;
        }
    }

    if (keyIndex == static_cast<size_t>(-1))
    {
        m_keysData.push_back(KeyData());
        m_keysData[m_keysData.size() - 1].m_keyCode = keyCode;
        m_keysData[m_keysData.size() - 1].m_name = GetName(keyCode);
        m_keysData[m_keysData.size() - 1].m_state = newState;
    }
    else if(newState == KeyState::k_KS_Release)//If its press then the first if should get it
    {
        m_keysData[keyIndex].m_keyCode = keyCode;
        m_keysData[keyIndex].m_name = GetName(keyCode);
        m_keysData[keyIndex].m_state = newState;
    }
}

//Updates the keys that are already in the vector
void SDL_InputModule2::UpdateCurrentKeys()
{
    for (size_t i = 0; i < m_keysData.size(); ++i)
    {
        if (m_keysData[i].m_state == KeyState::k_KS_Press)
        {
            m_keysData[i].m_state = KeyState::k_KS_Hold;
        }
        else if (m_keysData[i].m_state == KeyState::k_KS_Release)
        {
            m_keysData[i].m_state = KeyState::k_KS_Null;
            m_keysData[i].m_name = "UNKNOWN";
            m_keysData[i].m_keyCode = KeyCode::k_Unknown;
        }
    }
}


const char* SDL_InputModule2::GetName(KeyCode keyCode)
{
    switch (keyCode)
    {
    case KeyCode::k_Num_0:
        return "NUM 0";
    case KeyCode::k_Num_1:
        return "NUM 1";
    case KeyCode::k_Num_2:
        return "NUM 2";
    case KeyCode::k_Num_3:
        return "NUM 3";
    case KeyCode::k_Num_4:
        return "NUM 4";
    case KeyCode::k_Num_5:
        return "NUM 5";
    case KeyCode::k_Num_6:
        return "NUM 6";
    case KeyCode::k_Num_7:
        return "NUM 7";
    case KeyCode::k_Num_8:
        return "NUM 8";
    case KeyCode::k_Num_9:
        return "NUM 9";

    case KeyCode::k_Keypad_0:
        return "NUM_PAD 0";
    case KeyCode::k_Keypad_1:
        return "NUM_PAD 1";
    case KeyCode::k_Keypad_2:
        return "NUM_PAD 2";
    case KeyCode::k_Keypad_3:
        return "NUM_PAD 3";
    case KeyCode::k_Keypad_4:
        return "NUM_PAD 4";
    case KeyCode::k_Keypad_5:
        return "NUM_PAD 5";
    case KeyCode::k_Keypad_6:
        return "NUM_PAD 6";
    case KeyCode::k_Keypad_7:
        return "NUM_PAD 7";
    case KeyCode::k_Keypad_8:
        return "NUM_PAD 8";
    case KeyCode::k_Keypad_9:
        return "NUM_PAD 9";

    case KeyCode::k_A:
        return "A";
    case KeyCode::k_B:
        return "B";
    case KeyCode::k_C:
        return "C";
    case KeyCode::k_D:
        return "D";
    case KeyCode::k_E:
        return "E";
    case KeyCode::k_F:
        return "F";
    case KeyCode::k_G:
        return "G";
    case KeyCode::k_H:
        return "H";
    case KeyCode::k_I:
        return "I";
    case KeyCode::k_J:
        return "J";
    case KeyCode::k_K:
        return "K";
    case KeyCode::k_L:
        return "L";
    case KeyCode::k_M:
        return "M";
    case KeyCode::k_N:
        return "N";
    case KeyCode::k_O:
        return "O";
    case KeyCode::k_P:
        return "P";
    case KeyCode::k_Q:
        return "Q";
    case KeyCode::k_R:
        return "R";
    case KeyCode::k_S:
        return "S";
    case KeyCode::k_T:
        return "T";
    case KeyCode::k_U:
        return "U";
    case KeyCode::k_V:
        return "V";
    case KeyCode::k_W:
        return "W";
    case KeyCode::k_X:
        return "X";
    case KeyCode::k_Y:
        return "Y";
    case KeyCode::k_Z:
        return "Z";

    case KeyCode::k_Space:
        return "SPACE";
    case KeyCode::k_Ctrl_L:
        return "CTRL_L";
    case KeyCode::k_Esc:
        return "ESC";

    case KeyCode::k_LMB:
        return "LMB";
    case KeyCode::k_MMB:
        return "MMB";
    case KeyCode::k_RMB:
        return "RMB";

    default:
        return "UNKNOWN";
        break;
    }
}

KeyCode SDL_InputModule2::GetKeyCode(SDL_KeyCode keyCode)
{
    switch (keyCode)
    {
    case SDL_KeyCode::SDLK_0:
        return KeyCode::k_Num_0;
    case SDL_KeyCode::SDLK_1:
        return KeyCode::k_Num_1;
    case SDL_KeyCode::SDLK_2:
        return KeyCode::k_Num_2;
    case SDL_KeyCode::SDLK_3:
        return KeyCode::k_Num_3;
    case SDL_KeyCode::SDLK_4:
        return KeyCode::k_Num_4;
    case SDL_KeyCode::SDLK_5:
        return KeyCode::k_Num_5;
    case SDL_KeyCode::SDLK_6:
        return KeyCode::k_Num_6;
    case SDL_KeyCode::SDLK_7:
        return KeyCode::k_Num_7;
    case SDL_KeyCode::SDLK_8:
        return KeyCode::k_Num_8;
    case SDL_KeyCode::SDLK_9:
        return KeyCode::k_Num_9;

    case SDL_KeyCode::SDLK_KP_0:
        return KeyCode::k_Keypad_0;
    case SDL_KeyCode::SDLK_KP_1:
        return KeyCode::k_Keypad_1;
    case SDL_KeyCode::SDLK_KP_2:
        return KeyCode::k_Keypad_2;
    case SDL_KeyCode::SDLK_KP_3:
        return KeyCode::k_Keypad_3;
    case SDL_KeyCode::SDLK_KP_4:
        return KeyCode::k_Keypad_4;
    case SDL_KeyCode::SDLK_KP_5:
        return KeyCode::k_Keypad_5;
    case SDL_KeyCode::SDLK_KP_6:
        return KeyCode::k_Keypad_6;
    case SDL_KeyCode::SDLK_KP_7:
        return KeyCode::k_Keypad_7;
    case SDL_KeyCode::SDLK_KP_8:
        return KeyCode::k_Keypad_8;
    case SDL_KeyCode::SDLK_KP_9:
        return KeyCode::k_Keypad_9;

    case SDL_KeyCode::SDLK_a:
        return KeyCode::k_A;
    case SDL_KeyCode::SDLK_b:
        return KeyCode::k_B;
    case SDL_KeyCode::SDLK_c:
        return KeyCode::k_C;
    case SDL_KeyCode::SDLK_d:
        return KeyCode::k_D;
    case SDL_KeyCode::SDLK_e:
        return KeyCode::k_E;
    case SDL_KeyCode::SDLK_f:
        return KeyCode::k_F;
    case SDL_KeyCode::SDLK_g:
        return KeyCode::k_G;
    case SDL_KeyCode::SDLK_h:
        return KeyCode::k_H;
    case SDL_KeyCode::SDLK_i:
        return KeyCode::k_I;
    case SDL_KeyCode::SDLK_j:
        return KeyCode::k_J;
    case SDL_KeyCode::SDLK_k:
        return KeyCode::k_K;
    case SDL_KeyCode::SDLK_l:
        return KeyCode::k_L;
    case SDL_KeyCode::SDLK_m:
        return KeyCode::k_M;
    case SDL_KeyCode::SDLK_n:
        return KeyCode::k_N;
    case SDL_KeyCode::SDLK_o:
        return KeyCode::k_O;
    case SDL_KeyCode::SDLK_p:
        return KeyCode::k_P;
    case SDL_KeyCode::SDLK_q:
        return KeyCode::k_Q;
    case SDL_KeyCode::SDLK_r:
        return KeyCode::k_R;
    case SDL_KeyCode::SDLK_s:
        return KeyCode::k_S;
    case SDL_KeyCode::SDLK_t:
        return KeyCode::k_T;
    case SDL_KeyCode::SDLK_u:
        return KeyCode::k_U;
    case SDL_KeyCode::SDLK_v:
        return KeyCode::k_V;
    case SDL_KeyCode::SDLK_w:
        return KeyCode::k_W;
    case SDL_KeyCode::SDLK_x:
        return KeyCode::k_X;
    case SDL_KeyCode::SDLK_y:
        return KeyCode::k_Y;
    case SDL_KeyCode::SDLK_z:
        return KeyCode::k_Z;

    case SDL_KeyCode::SDLK_SPACE:
        return KeyCode::k_Space;
    case SDL_KeyCode::SDLK_LCTRL:
        return KeyCode::k_Ctrl_L;
    case SDL_KeyCode::SDLK_ESCAPE:
        return KeyCode::k_Esc;

    default:
        return KeyCode::k_Unknown;
        break;
    }
}