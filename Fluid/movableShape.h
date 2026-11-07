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
	bool dead = false; 

public:
	movableShape(Vector2f position, Vector2f speed = Vector2f(), Vector2f acceleration = Vector2f()) {
		this->position = position;
		this->speed = speed;
		this->acceleration = acceleration;
	}
	virtual ~movableShape() = default;


    void updateAcceleration(const Vector2f& offset) { acceleration = acceleration + offset; }
	void updatePosition(const Vector2f& offset) { position = position + offset; }
	void updateSpeed() { if(speed.magnitude() <= maxSpeed) { speed = speed + acceleration; } }

	void setAcceleration(const Vector2f& newAcceleration) { acceleration = newAcceleration; }
	void setPosition(const Vector2f& pos) { this->position = pos; }
	void setSpeed(const Vector2f& spd) { this->speed = spd; }

	void kill() { dead = true; }
	bool isDead() const { return dead; }

	const Vector2f& getAcceleration() { return acceleration; }
	const Vector2f& getPosition() { return position; }
	const Vector2f& getSpeed() { return speed; }
};

