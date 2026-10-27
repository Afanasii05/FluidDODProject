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
			shape->render();
		}

	}

	void changeNumberOfCircles(int number) {
		if (number > shapes.size()) {
			for (int i = 0;i < number - shapes.size();i++) {
				Circle* c = new Circle({ ourMath::randomFloat(200,1700), ourMath::randomFloat(200,800) }, { ourMath::randomFloat(-1,1) ,ourMath::randomFloat(-1,1) }, 1);
				addCircle(*c);
			}
		}
		else {
			for (int i = shapes.size() - 1; i >= number;i--) {
				delete shapes[i];
				shapes.pop_back();
			}
		}
	}
};