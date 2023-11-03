#pragma once
#include "CoreStructs.h"//      Point
#include <math.h>//             std math functions

class MathModule
{
public:
    static Point Normalize(const Point& p)
    {
        Point result = (p / sqrtf(p.x * p.x + p.y * p.y));


        //Checks for nan values and sets them to zero.
        if (result.x != result.x)
            result.x = 0;

        if (result.y != result.y)
            result.y = 0;

        return result;
    }

    static Point GetDirection(const Point& from, const Point& to)
    {
        return Normalize(to - from);
    }

    static float Distance(const Point& a, const Point& b)
    {
        return sqrtf(((b.x - a.x) * (b.x - a.x)) + ((b.y - a.y) * (b.y - a.y)));
    }
};