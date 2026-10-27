#pragma once
#include "Circle.h"
#include "vector"
#include <random>
#include "globalVariables.h"
class Physics
{
	std::vector<std::vector<movableShape*>> grid;
	std::vector<movableShape*> shapes;
	float width, height;
	float cellSize;
	int cols, rows;
	int squaredAddedCirclesRadius;

public:
	Physics() {
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
	void updateCollidedCircles() {
		const int dx[4] = { 1, -1, 0, 1 };
		const int dy[4] = { 0,  1, 1, 1 };

		for (int x = 0; x < cols; x++) {
			for (int y = 0; y < rows; y++) {
				std::vector<movableShape*>& cell = grid[x + cols * y];

				for (int k = 0; k < (int)cell.size(); k++) {
					for (int w = k + 1; w < (int)cell.size(); w++) {
						resolveColision(*cell[k], *cell[w]);
					}
				}

				for (int d = 0; d < 4; d++) {
					int nx = x + dx[d];
					int ny = y + dy[d];
					if (nx < 0 || nx >= cols || ny >= rows)
						continue;                          

					std::vector<movableShape*>& neighbor = grid[nx + cols * ny];
					for (int k = 0; k < (int)cell.size(); k++) {
						for (int w = 0; w < (int)neighbor.size(); w++) {
							resolveColision(*cell[k], *neighbor[w]);
						}
					}
				}
			}
		}
	}
	void resolveColision(movableShape& s1, movableShape& s2) {
		Vector2f p1 = s1.getPosition();
		Vector2f p2 = s2.getPosition();

		float dx = p2.x - p1.x;
		float dy = p2.y - p1.y;
		float dist2 = dx * dx + dy * dy;
		float minDist = 2.0f * globalVariables::circleRadius;  

		if (dist2 >= minDist * minDist)
			return;                                   
		float dist = std::sqrt(dist2);
		float nx = 1.0f, ny = 0.0f;                    
		if (dist > 0.0001f) {
			nx = dx / dist;
			ny = dy / dist;
		}

		float overlap = minDist - dist;
		s1.setPosition(Vector2f(p1.x - nx * overlap * 0.5f, p1.y - ny * overlap * 0.5f));
		s2.setPosition(Vector2f(p2.x + nx * overlap * 0.5f, p2.y + ny * overlap * 0.5f));

		Vector2f v1 = s1.getSpeed();
		Vector2f v2 = s2.getSpeed();
		float relVel = (v2.x - v1.x) * nx + (v2.y - v1.y) * ny;
		if (relVel > 0)
			return;                                   

		float restitution = 0.9f;                      
		float j = -(1.0f + restitution) * relVel / 2.0f;   
		s1.setSpeed(Vector2f(v1.x - j * nx, v1.y - j * ny));
		s2.setSpeed(Vector2f(v2.x + j * nx, v2.y + j * ny));

	}
	void addCircles(std::vector<movableShape*> addedShapes) {
		shapes.insert(shapes.end(), addedShapes.begin(), addedShapes.end());
	}
	void addCircle(Circle& cerc) {
		shapes.push_back(&cerc);
	}
	

	void update() {
		
		updateGrid();
		updateCollidedCircles();
		for (movableShape* shape : shapes) {
			if (shape->getPosition().x - globalVariables::circleRadius <= 0 || shape->getPosition().x + globalVariables::circleRadius >= globalVariables::screenWidth) {
				shape->setSpeed(shape->getSpeed() * Vector2f(-1, 1));
				shape->setAcceleration(shape->getAcceleration() * Vector2f(-1, 1));
			}
			if (shape->getPosition().y - globalVariables::circleRadius <= 0 || shape->getPosition().y + globalVariables::circleRadius >= globalVariables::screenHeight) {
				shape->setSpeed(shape->getSpeed() * Vector2f(1, -1));
				shape->setAcceleration(shape->getAcceleration() * Vector2f(1, -1));
			}
			shape->updateSpeed();
			shape->updatePosition(shape->getSpeed());
			shape->render();
		}
	}
	void changeNumberOfCircles(int number) {
		if (number > shapes.size()) {
			for (int i = 0;i < number - shapes.size();i++) {
				Circle* c = new Circle({ ourMath::randomFloat(200,1700), ourMath::randomFloat(200,800) }, { ourMath::randomFloat(-1,1) ,ourMath::randomFloat(-1,1) }, globalVariables::circleRadius);
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

