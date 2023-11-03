#pragma once

class IShooterHittable
{
public:
    virtual void OnHit(float dmg) = 0;
};