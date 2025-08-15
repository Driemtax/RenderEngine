#ifndef MATH_H
#define MATH_H

#include "../models/float2.cpp"

class math
{
public:
    static float Cross(float2 a, float2 b);
    static bool PointOnRightSideOfLine(float2 a, float2 b, float2 p);
    static bool PointInTriangle(float2 a, float2 b, float2 c, float2 p);
};

#endif