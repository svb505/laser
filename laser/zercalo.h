#pragma once
#include <vector>
#include "vec2.h"

struct Pos {
	float x, y;
	float angle;
	bool selected = false;
	Vec2 a, b;
};

class Zercalo {
public:
	std::vector<Pos> zercala;

	void init();
	void draw();
	void setSelected(int idx);
	void checkCollision();
};
