#include <vector>
#include <random>
#include "zercalo.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "getsegment.h"


void Zercalo::init() {
    zercala.clear();

    std::random_device rd;
    static std::mt19937 gen(rd());

    const int countX = 3;
    const int countY = 2;

    float minX = -200.0f;
    float maxX = 200.0f;
    float minY = -200.0f;
    float maxY = 200.0f;

    float cellW = (maxX - minX) / countX;
    float cellH = (maxY - minY) / countY;

    std::uniform_real_distribution<float> offsetX(0.2f, 0.8f);
    std::uniform_real_distribution<float> offsetY(0.2f, 0.8f);

    for (int iy = 0; iy < countY; iy++) {
        for (int ix = 0; ix < countX; ix++) {
            float x = minX + ix * cellW + cellW * offsetX(gen);
            float y = minY + iy * cellH + cellH * offsetY(gen);

            float halfLen = 20.0f;

            float c = cos(0.0f);
            float s = sin(0.0f);

            // горизонтальный отрезок по умолчанию
            Vec2 a = { -halfLen, 0.0f };
            Vec2 b = { halfLen, 0.0f };

            zercala.push_back({ x, y, 0.0f,false,a,b});
        }
    }
}
void Zercalo::setSelected(int idx) {
	for (auto& z : zercala) z.selected = false;
	zercala[idx].selected = true;
}
void Zercalo::draw() {
	float step = 15.0f;

	for (auto& z : zercala) {
        Vec2 a, b;

        GetSegment(z, a, b);

        if (z.selected) glColor3f(255.0f / 255.0f, 128.0f / 255.0f, 0.0f);
        else glColor3f(160.0f / 255.0f, 160.0f / 255.0f, 160.0f / 255.0f);

        glLineWidth(2.0f);

        glBegin(GL_LINES);
        glVertex2f(a.x, a.y);
        glVertex2f(b.x, b.y);
        glEnd();
	}
}