#include "math.h"

// Calculates the cross product of two vectors a and b
float math::Cross(float3 a, float3 b) {
    return a.x * b.y - a.y * b.x;
}

// checks if a point p is on the right side of the line between a and b
bool math::PointOnRightSideOfLine(float3 a, float3 b, float3 p) {
    float3 ap = float3{p.x - a.x, p.y - a.y, 0.0f};
    float3 ab = float3{b.x - a.x, b.y - a.y, 0.0f};
    return Cross(ab, ap) < 0;
}

// checks if a point p is inside the given triangle with points a,b,c.
// This is the case if point p is on every right side of the triangle
bool math::PointInTriangle(float3 a, float3 b, float3 c, float3 p) {
    bool ab = PointOnRightSideOfLine(a,b,p);
    bool bc = PointOnRightSideOfLine(b,c,p);
    bool ca = PointOnRightSideOfLine(c,a,p);
    return ab == bc && bc == ca;
}