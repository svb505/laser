#pragma once
#include "vec2.h"
#include <vector>

struct Pos;
class Laser;

class Target {
public:
	Vec2 position;
	float step = 20.0f;

	void init(std::vector<Pos>& zercala);
	void draw();
	bool checkCollision(Vec2 a, Vec2 b, Vec2 position, float step);
};