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
	float maxSpeed=1.0f;

	public:
	movableShape(Vector2f position, Vector2f acceleration = Vector2f()) {
		this->position = position;
		this->acceleration = acceleration;
	}
	virtual void updateAcceleration(Vector2f addedAcceleration) {
		acceleration = acceleration + addedAcceleration;
	}
	virtual void setAcceleration(Vector2f newAcceleration) {
		acceleration = newAcceleration;
	}
	virtual void updatePosition(Vector2f updatePosition) {
		position = position + updatePosition;
	}
	virtual void setPosition(Vector2f position) {
		this->position = position;
	}
	virtual void setSpeed(Vector2f speed) {
		this->speed = speed;
	}
	virtual void updateSpeed() {
		if(speed.magnitude()<=maxSpeed)
			speed = speed + acceleration;
	}
	Vector2f getSpeed() { return speed; }

};