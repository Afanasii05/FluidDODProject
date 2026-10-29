#pragma once
#include "Shader.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include "ourMath.h"
#include "Circle.h"
#include "globalVariables.h"
#include "imgui.h"
using namespace ourMath;
class PhysicsDod
{
	std::vector<std::vector<int>> grid;
	std::vector<Vector2f> posC;
	std::vector<Vector2f> speedC;
	float cellSize;
	int cols, rows;
	int squaredAddedCirclesRadius;
	GLuint Vao, Vbo;
public:
	PhysicsDod() {
		cellSize = 2 * globalVariables::circleRadius;
		rows = globalVariables::screenHeight / cellSize + 1;
		cols = globalVariables::screenWidth / cellSize + 1;
		grid.assign(rows * cols, {});
		squaredAddedCirclesRadius = cellSize * cellSize;

		//Generare de buffere pentru rendare
		glGenVertexArrays(1, &Vao);
		glGenBuffers(1, &Vbo);
		glBindVertexArray(Vao);
		glBindBuffer(GL_ARRAY_BUFFER, Vbo);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)0);  
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		
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
		for (int i = 0;i < posC.size();i++) {
			grid[cellIndex(posC[i])].push_back(i);
		}
	}

	void checkBetweenCells(std::vector<int>& cell, int x, int y) {
		if (x < 0 || x >= cols || y >= rows)
			return;
		std::vector<int>& neighbor = grid[x + cols * y];

		for (int a = 0; a < (int)cell.size(); a++)
			for (int b = 0; b < (int)neighbor.size(); b++)
				resolveColision(cell[a], neighbor[b]);
	}

	void updateCollidedCircles() {
		for (int y = 0; y < rows; y++) {
			for (int x = 0; x < cols; x++) {
				std::vector<int>& cell = grid[x + cols * y];
				if (cell.empty()) continue;

				for (int k = 0; k < (int)cell.size(); k++)
					for (int w = k + 1; w < (int)cell.size(); w++)
						resolveColision(cell[k], cell[w]);

				checkBetweenCells(cell, x + 1, y);
				checkBetweenCells(cell, x - 1, y + 1);
				checkBetweenCells(cell, x, y + 1);
				checkBetweenCells(cell, x + 1, y + 1);
			}
		}
	}
	void resolveColision(int indexC1, int indexC2) {
		//Verific initial daca colizioneaza
		Vector2f p1 = posC[indexC1];
		Vector2f p2 = posC[indexC2];

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
		posC[indexC1] = Vector2f(p1.x - nx * overlap / 2.0f, p1.y - ny * overlap / 2.0f);
		posC[indexC2] = Vector2f(p2.x + nx * overlap / 2.0f, p2.y + ny * overlap / 2.0f);

		//cat modific viteza dupa ciocnire in functie de normala ciocnirii.
		
		//puterea de apropiere una spre alta
		float relVel = (speedC[indexC2].x - speedC[indexC1].x) * nx + (speedC[indexC2].y - speedC[indexC1].y) * ny;
		if (relVel > 0)
			return;

		float elastic = 0.99f;
		float power = -(1.0f + elastic) * relVel / 2.0f;
		//aplic impulsul in sensul opus
		speedC[indexC1] = Vector2f(speedC[indexC1].x - power * nx, speedC[indexC1].y - power * ny);
		speedC[indexC2] = Vector2f(speedC[indexC2].x + power * nx, speedC[indexC2].y + power * ny);

	}
	


	void update() {

		updateGrid();
		updateCollidedCircles();
		for (int i = 0;i < posC.size();i++) {
			if (posC[i].x - globalVariables::circleRadius <= 0 || posC[i].x + globalVariables::circleRadius >= globalVariables::screenWidth) {
				speedC[i]=speedC[i] * Vector2f(-1, 1);
			}
			if (posC[i].y - globalVariables::circleRadius <= 0 || posC[i].y + globalVariables::circleRadius >= globalVariables::screenHeight) {
				speedC[i] = speedC[i] * Vector2f(1, -1);
			}
			posC[i] = posC[i] + speedC[i];
			
		}
	}
	void changeNumberOfCircles(int number) {
		if (number > posC.size()) {
			while (posC.size() < number) {
				posC.push_back({ ourMath::randomFloat(200, 1700), ourMath::randomFloat(200, 800) });
				speedC.push_back({ ourMath::randomFloat(-2, 2), ourMath::randomFloat(-2, 2) });
			}
		}
		else {
			while (posC.size() > number) {
				posC.pop_back();
				speedC.pop_back();
			}
		}

	}

	void render(Shader& sh) {
		sh.use();
		glBindVertexArray(Vao);
		glBindBuffer(GL_ARRAY_BUFFER, Vbo);
		glBufferData(GL_ARRAY_BUFFER, posC.size() * sizeof(Vector2f), posC.data(), GL_STREAM_DRAW);
		glDrawArrays(GL_POINTS, 0, (GLsizei)posC.size());                                            
		glBindVertexArray(0);
	}
	~PhysicsDod() {
		glDeleteBuffers(1, &Vbo);
		glDeleteVertexArrays(1, &Vao);
	}
};

