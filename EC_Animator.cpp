#include "EC_Animator.h"
#include "EC_Includes.h"
#include "EC_Sprite.h"

EC_Animator::EC_Animator()
    : Component()
    , m_pSprite(nullptr)
    , m_currAnmIndex(0)
    , m_anmSpeed(0)
    , m_deltaCount(0.0f)
    , m_currFrameNumber(0)
    , m_playAnimations(false)
    , m_loop(true)
{}

EC_Animator::~EC_Animator()
{}

void EC_Animator::Init()
{
    m_pSprite = m_pParent->GetComponent<EC_Sprite>();

    if (!m_pSprite)
        Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("EC_Sprite missing EC_Animator in \"" + m_pParent->GetName() + "\"").c_str());
}

void EC_Animator::Update(float delta)
{
    if (!m_playAnimations || !m_isActive)
        return;


    //check if its time to change frame
    if (m_deltaCount >= m_anmSpeed)
    {
        //current source rect
        Rect startRect = m_animations[m_currAnmIndex].m_rect;

        //If animation is at its last frame and we dont want to loop, return.
        if (m_currFrameNumber == m_animations[m_currAnmIndex].m_length && !m_loop)
            return;

        //Get frame number (X offset mult)
        m_currFrameNumber = m_currFrameNumber >= m_animations[m_currAnmIndex].m_length ? 1 : (m_currFrameNumber + 1);


        //Calculate final rect
        startRect.x += startRect.w * ((int)m_currFrameNumber - 1);

        //Update sprite's srcRect
        m_pSprite->SetSourceRect(startRect);
        m_deltaCount = 0.0f;
    }


    m_deltaCount += delta;
}

//Add a clip to the animator to be used later.
void EC_Animator::AddClip(const char* name, int length, Rect rect)
{
    m_animations.push_back(Animation(name, length, rect));
}

//Sets the ammount of seconds each frame shows in screen.
void EC_Animator::SetSpeed(float speed)
{
    m_anmSpeed = speed;
}


//Set a clip to be played.
//If the animator is not currently playing, the clip still gets set, so you will see its first frame frozen.
void EC_Animator::SetClip(const char* name, bool resetIfSame, bool loop)
{
    if (name == m_animations[m_currAnmIndex].m_name && !resetIfSame) return;

    for (size_t i = 0; i < m_animations.size(); ++i)
    {
        if (m_animations[i].m_name == name)
        {
            m_currAnmIndex = i;
            m_pSprite->SetSourceRect(m_animations[i].m_rect);
            m_loop = loop;
            return;
        }
    }
}

//Returns the name of the current clip set.
std::string EC_Animator::GetClipName()
{
    if (m_currAnmIndex < m_animations.size())
        return m_animations[m_currAnmIndex].m_name;

    Logger::Log(Logger::LOG_LEVEL::k_Error, std::string("\"m_currAnimationIndex\" is out of range in \"" + m_pParent->GetName() + "\"").c_str());

    return std::string("ERROR_INDEX_OOR");
}

void EC_Animator::PrintDebugData()
{
    std::cout << "name: " << m_animations[m_currAnmIndex].m_name << std::endl;
    std::cout << "length: " << m_animations[m_currAnmIndex].m_length << std::endl;
    std::cout << "rect: " << m_animations[m_currAnmIndex].m_rect.x << ", " << m_animations[m_currAnmIndex].m_rect.y << std::endl;
    std::cout << "CAI: " << m_currAnmIndex << std::endl;
    std::cout << "currFrame: " << m_currFrameNumber << std::endl;
}

//Decide wether to play the currently set clip or stop it in place.
void EC_Animator::Play(bool value)
{
    m_playAnimations = value;
}