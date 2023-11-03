#include "SDL_InputModule.h"
#include <iostream>

SDL_InputModule::SDL_InputModule()
{}

SDL_InputModule::~SDL_InputModule()
{}

/// <summary>
/// Returns the state of the desired key.
/// </summary>
/// <param name="keyCode">-KeyCode of the key you want to check.</param>
/// <returns>KeyState indicating the key's state.</returns>
KeyState SDL_InputModule::GetKeyState(KeyCode keyCode) const
{
    for (size_t i = 0; i < 64; ++i)
    {
        if (m_keyList[i].m_keyCode == keyCode)
            return m_keyList[i].m_state;
    }

    return KeyState::k_KS_Null;
}

bool SDL_InputModule::IsAnyKeyPressed(KeyState pressType)
{
    for (size_t i = 0; i < 64; ++i)
    {
        if (m_keyList[i].m_keyCode != KeyCode::k_Unknown)
        {
            if (m_keyList[i].m_state == pressType)
                return true;
        }
        else
            return false;
    }

    return false;
}

bool SDL_InputModule::Init()
{
    if (m_self == nullptr)
        m_self = this;

    return true;
}

/// <summary>
/// It makes sure to Update the Input data (keys array) each time is called.
/// This is used by Engine in its game loop.
/// </summary>
void SDL_InputModule::Update()
{
    static SDL_Event event;

    //Goes trough the keys array and "deletes" any key whos state is RELEASED
    ClearKeysWithState(KeyState::k_KS_Release);

    //Goes trough the keys array and updates the state of all PRESSED keys to the HOLD state
    for (int i = 0; i < 64; ++i)
    {
        if (m_keyList[i].m_state == KeyState::k_KS_Press)
            m_keyList[i].m_state = KeyState::k_KS_Hold;
    }

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EventType::SDL_KEYDOWN:
        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
        {
            //Search for the key in the array.
            int keyIndex = FindKeyInList(SDLKeyCodeToKeyCode((SDL_KeyCode)event.key.keysym.sym));

            //If the key is not found, its a new press, so add as PRESS.
            if (keyIndex < 0)
                AddKeyToList
                (
                    SDLKeyCodeToChar((SDL_KeyCode)event.key.keysym.sym),
                    KeyState::k_KS_Press,
                    SDLKeyCodeToKeyCode((SDL_KeyCode)event.key.keysym.sym)
                );
        }
            break;

        case SDL_EventType::SDL_KEYUP:
        case SDL_EventType::SDL_MOUSEBUTTONUP:
        {
            //Search for the key in the array and get its index.
            int keyIndex = FindKeyInList(SDLKeyCodeToKeyCode((SDL_KeyCode)event.key.keysym.sym));

            //Use the index to access the key and update its state to RELEASE.
            if (keyIndex >= 0)
                SetKeyState(keyIndex, KeyState::k_KS_Release);
        }
        break;

        case SDL_EventType::SDL_MOUSEMOTION:
        {
            m_mousePos.x = static_cast<float>(event.motion.x);
            m_mousePos.y = static_cast<float>(event.motion.y);
        }
        break;
        }
    }
}

void SDL_InputModule::DeInit()
{
    //.
}

/// <summary>
/// Finds a key in the array and returns its index or -1 if its not in it.
/// </summary>
/// <param name="keyCode"> The key to look for</param>
/// <returns>The key's index</returns>
int SDL_InputModule::FindKeyInList(KeyCode keyCode)
{
    for (int i = 0; i < 64; ++i)
    {
        if (m_keyList[i].m_keyCode == keyCode)
            return i;
    }

    return -1;
}

/// <summary>
/// Sets a Key to a speciffic state.
/// </summary>
/// <param name="listIndex">-The index on the array for the key whos state you want to change.</param>
/// <param name="newState">-The new state you want for your key.</param>
void SDL_InputModule::SetKeyState(int listIndex, KeyState newState)
{
    m_keyList[listIndex].m_state = newState;
}

/// <summary>
/// Goes trough the array of keys and clears all keys of the specified state.
/// </summary>
/// <param name="stateToKill">-State that defines which key will be cleared</param>
void SDL_InputModule::ClearKeysWithState(KeyState stateToKill)
{
    for (size_t i = 0; i <64; ++i)
    {
        if (m_keyList[i].m_state == stateToKill)
        {
            m_keyList[i].m_state = KeyState::k_KS_Null;
            m_keyList[i].m_pKeyName = "UNKNOWN";
            m_keyList[i].m_keyCode = KeyCode::k_Unknown;
        }
    }
}

/// <summary>
/// Adds a new key to the array.
/// </summary>
/// <param name="keyName">-The new key's keyName value.</param>
/// <param name="state">-The new key's state value.</param>
/// <param name="keyCode">-The new key's keyCode value.</param>
void SDL_InputModule::AddKeyToList(const char* keyName, KeyState state, KeyCode keyCode)
{
    for (int i = 0; i < 64; ++i)
    {
        if (m_keyList[i].m_state == KeyState::k_KS_Null)
        {
            m_keyList[i].m_pKeyName = keyName;
            m_keyList[i].m_state= state;
            m_keyList[i].m_keyCode = keyCode;
            break;
        }
    }
}

/// <summary>
/// Provided an SDL_KeyCode, it returns its equivalent const char* value
/// </summary>
/// <param name="keyCode">-SDL_KeyCode you want the const char* equivalent of.</param>
/// <returns></returns>
const char* SDL_InputModule::SDLKeyCodeToChar(SDL_KeyCode keyCode)
{
    switch (keyCode)
    {
    case SDL_KeyCode::SDLK_0:
        return "NUM 0";
    case SDL_KeyCode::SDLK_1:
        return "NUM 1";
    case SDL_KeyCode::SDLK_2:
        return "NUM 2";
    case SDL_KeyCode::SDLK_3:
        return "NUM 3";
    case SDL_KeyCode::SDLK_4:
        return "NUM 4";
    case SDL_KeyCode::SDLK_5:
        return "NUM 5";
    case SDL_KeyCode::SDLK_6:
        return "NUM 6";
    case SDL_KeyCode::SDLK_7:
        return "NUM 7";
    case SDL_KeyCode::SDLK_8:
        return "NUM 8";
    case SDL_KeyCode::SDLK_9:
        return "NUM 9";

    case SDL_KeyCode::SDLK_KP_0:
        return "NUM_PAD 0";
    case SDL_KeyCode::SDLK_KP_1:
        return "NUM_PAD 1";
    case SDL_KeyCode::SDLK_KP_2:
        return "NUM_PAD 2";
    case SDL_KeyCode::SDLK_KP_3:
        return "NUM_PAD 3";
    case SDL_KeyCode::SDLK_KP_4:
        return "NUM_PAD 4";
    case SDL_KeyCode::SDLK_KP_5:
        return "NUM_PAD 5";
    case SDL_KeyCode::SDLK_KP_6:
        return "NUM_PAD 6";
    case SDL_KeyCode::SDLK_KP_7:
        return "NUM_PAD 7";
    case SDL_KeyCode::SDLK_KP_8:
        return "NUM_PAD 8";
    case SDL_KeyCode::SDLK_KP_9:
        return "NUM_PAD 9";

    case SDL_KeyCode::SDLK_a:
        return "A";
    case SDL_KeyCode::SDLK_b:
        return "B";
    case SDL_KeyCode::SDLK_c:
        return "C";
    case SDL_KeyCode::SDLK_d:
        return "D";
    case SDL_KeyCode::SDLK_e:
        return "E";
    case SDL_KeyCode::SDLK_f:
        return "F";
    case SDL_KeyCode::SDLK_g:
        return "G";
    case SDL_KeyCode::SDLK_h:
        return "H";
    case SDL_KeyCode::SDLK_i:
        return "I";
    case SDL_KeyCode::SDLK_j:
        return "J";
    case SDL_KeyCode::SDLK_k:
        return "K";
    case SDL_KeyCode::SDLK_l:
        return "L";
    case SDL_KeyCode::SDLK_m:
        return "M";
    case SDL_KeyCode::SDLK_n:
        return "N";
    case SDL_KeyCode::SDLK_o:
        return "O";
    case SDL_KeyCode::SDLK_p:
        return "P";
    case SDL_KeyCode::SDLK_q:
        return "Q";
    case SDL_KeyCode::SDLK_r:
        return "R";
    case SDL_KeyCode::SDLK_s:
        return "S";
    case SDL_KeyCode::SDLK_t:
        return "T";
    case SDL_KeyCode::SDLK_u:
        return "U";
    case SDL_KeyCode::SDLK_v:
        return "V";
    case SDL_KeyCode::SDLK_w:
        return "W";
    case SDL_KeyCode::SDLK_x:
        return "X";
    case SDL_KeyCode::SDLK_y:
        return "Y";
    case SDL_KeyCode::SDLK_z:
        return "Z";
    
    case SDL_KeyCode::SDLK_SPACE:
        return "SPACE";
    case SDL_KeyCode::SDLK_LCTRL:
        return "CTRL_L";

    default:
        return "UNKNOWN";
        break;
    }
}

/// <summary>
/// Provided an SDL_KeyCode, it returns its equivalent KeyCode
/// </summary>
/// <param name="keyCode">-SDL_KeyCode you want the KeyCode equivalent of.</param>
/// <returns></returns>
KeyCode SDL_InputModule::SDLKeyCodeToKeyCode(SDL_KeyCode keyCode)
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

    default:
        return KeyCode::k_Unknown;
        break;
    }
}

void SDL_InputModule::DEBUG_PrintArray()
{
    system("cls");

    for (int i = 0; i < 9; ++i)
    {
        std::cout << m_keyList[i].m_pKeyName <<", " << (int)m_keyList[i].m_state << std::endl;
    }
}

Point SDL_InputModule::GetMousePos()
{
    return m_mousePos;
}