#pragma once
#include "Drawable.h"
#include "ourMath.h"
using namespace ourMath;
class movableShape : public Drawable
{
	protected:
	Vector2f acceleration;
	Vector2f position;
	Vector2f speed;
	float maxSpeed = 3.0f;

	public:
	movableShape(Vector2f position, Vector2f acceleration = Vector2f()) {
		this->position = position;
		this->acceleration = acceleration;
	}
    void updateAcceleration(Vector2f addedAcceleration) {
		acceleration = acceleration + addedAcceleration;
	}
	void setAcceleration(Vector2f newAcceleration) {
		acceleration = newAcceleration;
	}
	void updatePosition(Vector2f updatePosition) {
		position = position + updatePosition;
	}
	void setPosition(Vector2f position) {
		this->position = position;
	}
	void setSpeed(Vector2f speed) {
		this->speed = speed;
	}
	void updateSpeed() {
		if(speed.magnitude()<=maxSpeed)
			speed = speed + acceleration;
	}
	Vector2f getAcceleration() { return acceleration; }
	Vector2f getSpeed() { return speed; }
	Vector2f getPosition() { return position; }
};

