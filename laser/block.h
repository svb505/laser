#pragma once
#include <vector>
#include "vec2.h"

class Block {
public:
	std::vector<Vec2> blocks;

	void draw();
	void init(int ecranh);
};