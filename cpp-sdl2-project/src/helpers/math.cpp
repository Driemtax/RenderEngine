#include "math.h"

float math::Cross(float2 a, float2 b) {
    return a.x * b.y - a.y * b.x;
}

bool math::PointOnRightSideOfLine(float2 a, float2 b, float2 p) {
    float2 ap = float2{p.x - a.x, p.y - a.y};
    float2 ab = float2{b.x - a.x, b.y - a.y};
    return Cross(ab, ap) < 0;
}

bool math::PointInTriangle(float2 a, float2 b, float2 c, float2 p) {
    bool ab = PointOnRightSideOfLine(a,b,p);
    bool bc = PointOnRightSideOfLine(b,c,p);
    bool ca = PointOnRightSideOfLine(c,a,p);
    return ab == bc && bc == ca;
}