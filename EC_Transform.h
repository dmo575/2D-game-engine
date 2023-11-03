#pragma once
#include "Component.h"//        Superclass
#include "CoreStructs.h"//      Point

class Entity;

class EC_Transform : public Component
{
public:
    EC_Transform();
    EC_Transform(float x, float y);
    virtual ~EC_Transform() override;

    Point GetPos();
    void SetPos(float x, float y);
    void SetPos(int x, int y);
    void SetPos(const Point& newPos);
    void AddPos(float x, float y);
    void AddPos(Point toAdd);


    Point GetScale();
    void SetScale(float x, float y);
    void SetScale(const Point& newScale);
    void AddScale(float x, float y);

private:
    virtual void Init() override;
    virtual void Update(float delta) override;

private:
    Point m_pos;
    Point m_scale;
};