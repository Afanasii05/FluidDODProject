#pragma once
#include "Circle.h"
#include "vector"
class Physics
{
	std::vector<movableShape*> shapes;

public:
	void addCircles(std::vector<movableShape*> addedShapes) {
		shapes.insert(shapes.end(), addedShapes.begin(), addedShapes.end());
	}
	void addCircle(Circle& cerc) {
		shapes.push_back(&cerc);
	}
	

	void update() {
		for (movableShape* shape : shapes) {
			shape->updateSpeed();
			shape->updatePosition(shape->getSpeed());
		}

	}
};