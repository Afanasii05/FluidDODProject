#pragma once
#include "imgui.h"
namespace ourMath
{
	struct Vector2f {
		float x, y;
		Vector2f(float x, float y) { this->x = x; this->y = y; };
		Vector2f() { x = 0;y = 0; }
		Vector2f operator+(const Vector2f other) {
			return Vector2f(x + other.x, y + other.y);
		}
		Vector2f& operator=(const Vector2f& other) {
			if (this == &other) {
				return *this;
			}
			this->x = other.x;
			this->y = other.y;
			return *this;
		}
		operator ImVec2() const { return ImVec2(x, y); }
	};
	


};

