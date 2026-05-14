#pragma once

struct Vec2 {
	double x, y;
    Vec2 operator+(const Vec2& other) const {
        return { x + other.x, y + other.y };
    }
    Vec2 operator-(const Vec2& other) const {
        return { x - other.x, y - other.y };
    }
};
