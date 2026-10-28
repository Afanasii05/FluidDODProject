#pragma once
#include "Circle.h"
#include "vector"
#include <random>
#include <algorithm>
#include "globalVariables.h"
class Physics
{
	std::vector<std::vector<movableShape*>> grid;
	std::vector<movableShape*> shapes;
	float cellSize;
	int cols, rows;
	int squaredAddedCirclesRadius;

public:
	Physics() {
		//setez ca marimea unui grid sa fie cat diametrul unei bile(bilele trebuie sa aibe toate aceeasi marime)
		cellSize =  2* globalVariables::circleRadius;
		rows = globalVariables::screenHeight /cellSize + 1;
		cols = globalVariables::screenWidth /cellSize + 1;
		grid.assign(rows *cols, {});
		squaredAddedCirclesRadius = cellSize*cellSize;
	}
	int cellIndex(Vector2f position) {
		int x = std::clamp(int(position.x / cellSize), 0, cols - 1);
		int y = std::clamp(int(position.y / cellSize), 0, rows - 1);
		return x + y * cols;
	}

	void updateGrid() {
		for (auto& gridCell : grid) {
			gridCell.clear();
		}
		for (movableShape* shape : shapes) {
			grid[cellIndex(shape->getPosition())].push_back(shape);
		}
	}

	void checkBetweenCells(std::vector<movableShape*>& cell, int x, int y) {
		if (x < 0 || x >= cols || y >= rows)
			return;
		std::vector<movableShape*>& neighbor = grid[x + cols * y];
		for (movableShape* a : cell)
			for (movableShape* b : neighbor)
				resolveColision(*a, *b);
	}

	void updateCollidedCircles() {
		for (int y = 0; y < rows; y++) {
			for (int x = 0; x < cols; x++) {
				std::vector<movableShape*>& cell = grid[x + cols * y];
				if (cell.empty()) continue;
				for (int k = 0; k < (int)cell.size(); k++)
					for (int w = k + 1; w < (int)cell.size(); w++)
						resolveColision(*cell[k], *cell[w]);
				//din cauza ca deja verific 4 vecini,  nu ii iau si pe cei de sus
				checkBetweenCells(cell, x+1, y);
				checkBetweenCells(cell, x-1, y +1);
				checkBetweenCells(cell, x, y +1);
				checkBetweenCells(cell, x +1, y +1);
			}
		}
	}
	void resolveColision(movableShape& s1, movableShape& s2) {
		//Verific initial daca colizioneaza
		Vector2f p1 = s1.getPosition();
		Vector2f p2 = s2.getPosition();

		float dirX = p2.x - p1.x;
		float dirY = p2.y - p1.y;
		float dist2 = dirX * dirX + dirY * dirY;
		float minDist = 2.0f * globalVariables::circleRadius;  

		if (dist2 >= minDist * minDist)
			return;       

		//Daca colizionez, cat le mut ca sa nu mai colizioneze
		float dist = std::sqrt(dist2);
		float nx = 1.0f, ny = 0.0f;   
		if (dist > 0.0001f) {
			//calculez normala directiei ca sa stiu in ce directie scot bilele 
			nx = dirX / dist;
			ny = dirY / dist;
		}
		//le pozitionez pe media overlapului ca sa nu mai fie una in alta.
		float overlap = minDist - dist;
		s1.setPosition(Vector2f(p1.x - nx * overlap /2.0f, p1.y - ny * overlap /2.0f));
		s2.setPosition(Vector2f(p2.x + nx * overlap /2.0f, p2.y + ny * overlap /2.0f));

		//cat modific viteza dupa ciocnire in functie de normala ciocnirii.
		Vector2f v1 = s1.getSpeed();
		Vector2f v2 = s2.getSpeed();
		//puterea de apropiere una spre alta
		float relVel = (v2.x - v1.x) * nx + (v2.y - v1.y) * ny;
		if (relVel > 0)
			return;                                   

		//o valoare de elasticitate pentru o coliziune care pierde putin din energie
		float elastic = 0.99f;                      
		float power = -(1.0f + elastic) * relVel / 2.0f;   
		//aplic impulsul in sensul opus
		s1.setSpeed(Vector2f(v1.x - power * nx, v1.y - power * ny));
		s2.setSpeed(Vector2f(v2.x + power * nx, v2.y + power * ny));

	}
	void addCircles(std::vector<movableShape*> addedShapes) {
		shapes.insert(shapes.end(), addedShapes.begin(), addedShapes.end());
	}
	void addCircle(Circle* cerc) {
		shapes.push_back(cerc);
	}
	

	void update() {
		
		updateGrid();
		updateCollidedCircles();
		for (movableShape* shape : shapes) {
			if (shape->getPosition().x - globalVariables::circleRadius <= 0 || shape->getPosition().x + globalVariables::circleRadius >= globalVariables::screenWidth) {
				shape->setSpeed(shape->getSpeed() * Vector2f(-1, 1));
			}
			if (shape->getPosition().y - globalVariables::circleRadius <= 0 || shape->getPosition().y + globalVariables::circleRadius >= globalVariables::screenHeight) {
				shape->setSpeed(shape->getSpeed() * Vector2f(1, -1));
			}
			shape->updatePosition(shape->getSpeed());
			shape->render();
		}
	}
	void changeNumberOfCircles(int number) {
		if (number > shapes.size()) {
			for (int i = 0;i < number - shapes.size();i++) {
				Circle* c = new Circle({ ourMath::randomFloat(200,1700), ourMath::randomFloat(200,800) }, {0,0}, globalVariables::circleRadius);
				c->setSpeed({ ourMath::randomFloat(-3, 3), ourMath::randomFloat(-3, 3) });
				addCircle(c);
			}
		}
		else {
			for (int i = shapes.size() - 1; i >= number;i--) {
				delete shapes[i];
				shapes.pop_back();
			}
		}
	}
	~Physics() {
		for (movableShape* shape : shapes)
			delete shape;
	}
};

