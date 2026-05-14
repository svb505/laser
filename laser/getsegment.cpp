#include "vec2.h"
#include "zercalo.h"
#include <cmath>

void GetSegment(const Pos& z, Vec2& outA, Vec2& outB) {
    Vec2 localA = z.a;
    Vec2 localB = z.b;

    float c = cos(z.angle);
    float s = sin(z.angle);

    Vec2 ra = {
        localA.x * c - localA.y * s,
        localA.x * s + localA.y * c
    };

    Vec2 rb = {
        localB.x * c - localB.y * s,
        localB.x * s + localB.y * c
    };

    outA = { ra.x + z.x, ra.y + z.y };
    outB = { rb.x + z.x, rb.y + z.y };
}