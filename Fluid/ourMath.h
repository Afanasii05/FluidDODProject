#pragma once
#include "imgui.h"
#include <cmath>
#include <random>
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
		Vector2f operator*(float scalar) const {
			return Vector2f(x * scalar, y * scalar);

		}
		Vector2f operator*(const Vector2f& other) {
			return Vector2f(x * other.x, y * other.y);
		}
		operator ImVec2() const { return ImVec2(x, y); }
		float magnitude() {
			return sqrt(x * x + y * y);
		}
		
	};

	inline std::mt19937& rng() {
		static std::mt19937 gen(std::random_device{}());
		return gen;
	}

	inline float randomFloat(float min, float max) {
		std::uniform_real_distribution<float> dist(min, max);
		return dist(rng());
	}

	inline int randomInt(int min, int max) {
		std::uniform_int_distribution<int> dist(min, max);
		return dist(rng());
	}


};

