#include <GLFW/glfw3.h>
#include <vector>
#include "laser.h"
#include <cmath>
#include "zercalo.h"
#include "getsegment.h"

Vec2 Laser::RotatePoint(Vec2 p, float angle){
    float c = cos(angle);
    float s = sin(angle);

    return { p.x * c - p.y * s, p.x * s + p.y * c};
}
void Laser::GetTriangle(const Pos& z, Vec2 out[3]){
    float step = 20.0f;

    Vec2 local[3] = {
        {0.0f, step},
        {-step, 0.0f},
        {step, 0.0f}
    };

    for (int i = 0; i < 3; i++)
    {
        Vec2 r = RotatePoint(local[i], z.angle);

        out[i] = {
            r.x + z.x,
            r.y + z.y
        };
    }
}
Vec2 Laser::GetNormal(Vec2 a, Vec2 b, Vec2 rayDir){
    Vec2 edge = { b.x - a.x, b.y - a.y };

    // перпендикуляр
    Vec2 normal = { -edge.y, edge.x };

    // нормализация
    double len = sqrt(normal.x * normal.x + normal.y * normal.y);
    normal.x /= len;
    normal.y /= len;

    double dot = normal.x * rayDir.x + normal.y * rayDir.y;
    if (dot > 0.0f) {
        normal.x = -normal.x;
        normal.y = -normal.y;
    }



    return normal;
}
Vec2 Laser::Reflect(Vec2 dir, Vec2 normal){
    float dot = dir.x * normal.x + dir.y * normal.y;

    return {
        dir.x - 2.0f * dot * normal.x,
        dir.y - 2.0f * dot * normal.y
    };
}
void Laser::update(const std::vector<Pos>& mirrors, const std::vector<Vec2>& blocks){
    segments.clear();

    Vec2 origin = start;
    Vec2 dir = { cos(angle), sin(angle) };

    const int maxBounces = 40;

    for (int bounce = 0; bounce < maxBounces; bounce++){
        float closestT = 999999.0f;
        Vec2 hitPoint;
        Vec2 hitNormal;
        bool hit = false;
        bool blockHit = false;

        for (auto& z : mirrors) {
            Vec2 a, b;
            GetSegment(z, a, b);

            double t;
            if (RaySegmentIntersection(origin, dir, a, b, t)) {

                Vec2 normal = GetNormal(a, b, dir);

                double ndot = dir.x * normal.x + dir.y * normal.y;

                if (ndot >= 0.0f) continue;

                if (t < closestT) {
                    closestT = t;
                    hit = true;

                    hitPoint = {
                        origin.x + dir.x * t,
                        origin.y + dir.y * t
                    };

                    hitNormal = normal;
                }
            }
        }
        for (auto& b : blocks) {
            float size = 20.0f;

            Vec2 quad[4] = {
                {b.x - size, b.y - size},
                {b.x + size, b.y - size},
                {b.x + size, b.y + size * 5},
                {b.x - size, b.y + size * 5}
            };

            for (int i = 0; i < 4; i++) {
                Vec2 a = quad[i];
                Vec2 c = quad[(i + 1) % 4];

                double t;
                if (RaySegmentIntersection(origin, dir, a, c, t)) {
                    if (t < closestT) {
                        closestT = t;
                        hit = true;
                        blockHit = true;

                        hitPoint = {
                            origin.x + dir.x * t,
                            origin.y + dir.y * t
                        };
                    }
                }
            }
        }
        if (hit){
            segments.push_back({ origin, hitPoint });

            if (blockHit)  break;

            dir = Reflect(dir, hitNormal);

            float len = sqrt(dir.x * dir.x + dir.y * dir.y);
            dir.x /= len;
            dir.y /= len;

            const float EPS = 0.01f;
            origin = { hitPoint.x + dir.x * EPS, hitPoint.y + dir.y * EPS };
        }
        else{
            Vec2 end = {origin.x + dir.x * 1000.0f,origin.y + dir.y * 1000.0f};

            segments.push_back({ origin, end });
            break;
        }
    }
}
void Laser::init(float x, float y,float angle) {
	this->angle = angle;
    start = { x, y };
}
void Laser::updateDir() {
    dir = { cos(angle), sin(angle) };
}
bool Laser::RaySegmentIntersection(Vec2 rayOrigin, Vec2 rayDir, Vec2 a, Vec2 b, double& t){
    Vec2 v1 = { rayOrigin.x - a.x, rayOrigin.y - a.y };
    Vec2 v2 = { b.x - a.x, b.y - a.y };
    Vec2 v3 = { -rayDir.y, rayDir.x };

    double dot = v2.x * v3.x + v2.y * v3.y;
    if (fabs(dot) < 0.001f) return false;

    double t1 = (v2.x * v1.y - v2.y * v1.x) / dot;
    double t2 = (v1.x * v3.x + v1.y * v3.y) / dot;

    const float MIN_T = 0.0001f;

    if (t1 >= MIN_T && t2 >= 0.0f && t2 <= 1.0f) {
        t = t1;
        return true;
    }

    return false;
}
void Laser::draw(){
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_LINES);

    for (auto& s : segments){
        glVertex2f(s.a.x, s.a.y);
        glVertex2f(s.b.x, s.b.y);
    }

    glEnd();
}