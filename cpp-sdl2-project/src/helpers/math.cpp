#include "math.h"

// Calculates the cross product of two vectors a and b
float math::Cross(float2 a, float2 b) {
    return a.x * b.y - a.y * b.x;
}

// checks if a point p is on the right side of the line between a and b
bool math::PointOnRightSideOfLine(float2 a, float2 b, float2 p) {
    float2 ap = float2{p.x - a.x, p.y - a.y};
    float2 ab = float2{b.x - a.x, b.y - a.y};
    return Cross(ab, ap) < 0;
}

// checks if a point p is inside the given triangle with points a,b,c.
// This is the case if point p is on every right side of the triangle
bool math::PointInTriangle(float2 a, float2 b, float2 c, float2 p) {
    bool ab = PointOnRightSideOfLine(a,b,p);
    bool bc = PointOnRightSideOfLine(b,c,p);
    bool ca = PointOnRightSideOfLine(c,a,p);
    return ab == bc && bc == ca;
}