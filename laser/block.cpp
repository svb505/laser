#include <GLFW/glfw3.h>
#include <vector>
#include "vec2.h"
#include "block.h"
#include <random>

void Block::init(int ecranh) {
	float height = ecranh / 3;

	blocks.push_back({ -300.0f,height});
	blocks.push_back({ -250.0f ,0.0f});
	blocks.push_back({ -180.0f,-height});
}
void Block::draw() {
	float step = 20.0f;

	for (auto& b : blocks) {
		glPushMatrix();
		glTranslatef(b.x, b.y, 0);

		glColor3f(0.7f, 0.7f, 0.7f);
		glBegin(GL_QUADS);

		glVertex2f(-step, step * 5);
		glVertex2f(step, step * 5);
		glVertex2f(step, -step);
		glVertex2f(-step, -step);

		glEnd();

		glPopMatrix();
	}
}