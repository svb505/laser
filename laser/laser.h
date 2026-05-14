#pragma once
#include <vector>
#include "vec2.h"

class Zercalo;
struct Pos;

struct Segment {
	Vec2 a;
	Vec2 b;
};

class Laser {
public:
	Vec2 start;
	Vec2 dir;
	float angle;

	int record = 0;

	std::vector<Segment> segments;

	void init(float x, float y,float angle);
	void draw();
	void updateDir();
	bool RaySegmentIntersection(Vec2 rayOrigin, Vec2 rayDir, Vec2 a, Vec2 b, double& t);
	Vec2 GetNormal(Vec2 a, Vec2 b, Vec2 rayDir);
	Vec2 Reflect(Vec2 dir, Vec2 normal);
	void update(const std::vector<Pos>& mirrors, const std::vector<Vec2>& blocks);
	void GetTriangle(const Pos& z, Vec2 out[3]);
	Vec2 RotatePoint(Vec2 p, float angle);
};
