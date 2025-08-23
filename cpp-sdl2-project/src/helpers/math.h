#ifndef MATH_H
#define MATH_H

#include "../models/float3.cpp"

class math
{
public:
    static float Cross(float3 a, float3 b);
    static bool PointOnRightSideOfLine(float3 a, float3 b, float3 p);
    static bool PointInTriangle(float3 a, float3 b, float3 c, float3 p);
};

#endif