#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Rect
#include <vector>//             std::vector
#include <string>//             std:string

class Entity;
class EC_Sprite;

class EC_Animator : public Component
{
    /*
    * Name: Used to identifiy an animation in the animations vector.
    * Length: In frames.
    * Rect: Defines the size and start position of the animation in the image.
    */
private:
    struct Animation
    {
    public:
        Animation(const char* name, int length, Rect rect)
            : m_name(name)
            , m_length(length)
            , m_rect(rect)
        {}

    public:
        std::string m_name;
        int m_length;
        Rect m_rect;
    };

public:
    EC_Animator();
    virtual ~EC_Animator() override;


    void AddClip(const char* id, int length, Rect rect);
    void SetSpeed(float speed);
    void SetClip(const char* name, bool resetIfSame = false, bool loop = true);
    std::string GetClipName();
    void Play(bool value);


    //DEBUG:
    void PrintDebugData();

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    EC_Sprite* m_pSprite;
    std::vector<Animation> m_animations;
    size_t m_currAnmIndex;
    float m_anmSpeed;//How long does a frame stays as the current frame, in seconds.
    float m_deltaCount;
    int m_currFrameNumber;//Current X frame
    bool m_playAnimations;
    bool m_loop;
};