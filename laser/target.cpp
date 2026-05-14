#include <GLFW/glfw3.h>
#include "target.h"
#include <random>
#include "Vec2.h"
#include "laser.h"
#include <iostream>
#include "zercalo.h"

bool pointInSquare(Vec2 p, float minX, float minY, float maxX, float maxY) {
	return p.x >= minX && p.x <= maxX &&
		p.y >= minY && p.y <= maxY;
}
float cross(Vec2 a, Vec2 b) {
	return a.x * b.y - a.y * b.x;
}
int orientation(Vec2 a, Vec2 b, Vec2 c) {
	float val = cross(b - a, c - a);
	if (val == 0) return 0;       // collinear
	return (val > 0) ? 1 : 2;     // left / right
}
bool onSegment(Vec2 a, Vec2 b, Vec2 p) {
	return p.x >= std::min(a.x, b.x) && p.x <= std::max(a.x, b.x) &&
		p.y >= std::min(a.y, b.y) && p.y <= std::max(a.y, b.y);
}
bool segmentsIntersect(Vec2 p1, Vec2 p2, Vec2 q1, Vec2 q2) {
	int o1 = orientation(p1, p2, q1);
	int o2 = orientation(p1, p2, q2);
	int o3 = orientation(q1, q2, p1);
	int o4 = orientation(q1, q2, p2);

	if (o1 != o2 && o3 != o4)
		return true;

	// collinear cases
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;
	if (o2 == 0 && onSegment(p1, p2, q2)) return true;
	if (o3 == 0 && onSegment(q1, q2, p1)) return true;
	if (o4 == 0 && onSegment(q1, q2, p2)) return true;

	return false;
}

void Target::init(std::vector<Pos>& zercala) {
	std::random_device rd;
	static std::mt19937 gen(rd());

	std::uniform_real_distribution<float> distY(-350.0f, 350.0f);
	std::uniform_real_distribution<float> distX(-350.0f, 350.0f);
	
	float x = distX(gen);
	float y = distY(gen);

	for (auto& z : zercala) {
		if (z.x == x || z.y == y) {
			x = distX(gen);
			y = distY(gen);
		}
		else {
			position.x = distX(gen);
			position.y = distY(gen);
			break;
		}
	}

	
}
void Target::draw() {
	glPushMatrix();
	glTranslatef(position.x, position.y, 0);

	glColor3f(1.5f, 0, 0);
	glBegin(GL_QUADS);

	glVertex2f(-step,step);
	glVertex2f(step, step);
	glVertex2f(step, -step);
	glVertex2f(-step, -step);

	glEnd();

	glPopMatrix();
}
bool Target::checkCollision(Vec2 a, Vec2 b, Vec2 position, float step) {
	float minX = position.x - step;
	float minY = position.y - step;
	float maxX = position.x + step;
	float maxY = position.y + step;

	// 1. точка внутри
	if (pointInSquare(a, minX, minY, maxX, maxY) ||
		pointInSquare(b, minX, minY, maxX, maxY))
		return true;

	// 2. стороны квадрата
	Vec2 bl = { minX, minY };
	Vec2 br = { maxX, minY };
	Vec2 tr = { maxX, maxY };
	Vec2 tl = { minX, maxY };

	// 3. проверка с каждой стороной
	return segmentsIntersect(a, b, bl, br) ||
		segmentsIntersect(a, b, br, tr) ||
		segmentsIntersect(a, b, tr, tl) ||
		segmentsIntersect(a, b, tl, bl);
}