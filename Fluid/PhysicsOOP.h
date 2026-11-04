#pragma once
#include "Shader.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include "ourMath.h"
#include "globalVariables.h"
#include "Circle.h"
#include "imgui.h"
using namespace ourMath;

class PhysicsDOD;

class PhysicsOOP
{
	std::vector<std::vector<int>> grid;
	std::vector<movableShape*> shapes;
	float cellSize, squaredAddedCirclesRadius;
	int cols, rows;
	GLuint Vao, Vbo;

public:
	PhysicsOOP();
	PhysicsOOP(const PhysicsDOD& dod);
	~PhysicsOOP();



	// getters & setters
	[[nodiscard]] const std::vector<std::vector<int>>& getGrid() const { return grid; }
	[[nodiscard]] const std::vector<movableShape*>& getShapes() const { return shapes; }
	[[nodiscard]] float getCellSize() const { return cellSize; }
	[[nodiscard]] int getCols() const { return cols; }
	[[nodiscard]] int getRows() const { return rows; }
	[[nodiscard]] float getSquaredRadius() const { return squaredAddedCirclesRadius; }
	[[nodiscard]] GLuint getVAO() const { return Vao; }
	[[nodiscard]] GLuint getVBO() const { return Vbo; }

	void setGrid(const std::vector<std::vector<int>>& grd) { this->grid = grd; }
	void setShapes(const std::vector<movableShape*>& sh) { this->shapes = sh; }
	void setCellSize(float cell_size) { cellSize = cell_size; }
	void setCols(int cls) { this->cols = cls; }
	void setRows(int rws) { this->rows = rws; }
	void setSquaredRadius(float sqr_rad) { squaredAddedCirclesRadius = sqr_rad; }
	void setVAO(GLuint vao) { Vao = vao; }
	void setVBO(GLuint vbo) { Vbo = vbo; }



	// .cpp functions
	void update();
	void updateGrid();
	void render(Shader& sh);
	void addCircle(Circle* cerc);
	void changeNumberOfCircles(int number);
	void addCircles(std::vector<movableShape*> addedShapes);



	// inline functions
	int cellIndex(Vector2f position) const {
		int x = std::clamp(int(position.x / cellSize), 0, cols - 1);
		int y = std::clamp(int(position.y / cellSize), 0, rows - 1);
		return x + y * cols;
	}

	void checkBetweenCells(std::vector<int>& cell, int x, int y) {
	    if (x < 0 || x >= cols || y >= rows)
	        return;
	    std::vector<int>& neighbor = grid[x + cols * y];
	    for (int a : cell)
	        for (int b : neighbor)
	            resolveColision(*shapes[a], *shapes[b]);
	}

	void updateCollidedCircles() {
	    for (int y = 0; y < rows; y++) {
	        for (int x = 0; x < cols; x++) {
	            std::vector<int>& cell = grid[x + cols * y];
	            if (cell.empty()) continue;

	            for (int k = 0; k < (int)cell.size(); k++)
	                for (int w = k + 1; w < (int)cell.size(); w++)
	                    resolveColision(*shapes[cell[k]], *shapes[cell[w]]);

	            checkBetweenCells(cell, x + 1, y);
	            checkBetweenCells(cell, x - 1, y + 1);
	            checkBetweenCells(cell, x, y + 1);
	            checkBetweenCells(cell, x + 1, y + 1);
	        }
	    }
	}

	void resolveColision(movableShape& s1, movableShape& s2) {
	    Vector2f p1 = s1.getPosition(), p2 = s2.getPosition();
	    float dirX = p2.x - p1.x, dirY = p2.y - p1.y;
	    float dist_sqr = dirX * dirX + dirY * dirY;

		// verifica daca cele doua cercuri sunt in coliziune
	    if (dist_sqr >= squaredAddedCirclesRadius) { return; }

	    // distanta necesara a.i. sa nu mai colizioneze
	    float dist = std::sqrt(dist_sqr);
	    float nx = 1.0f, ny = 0.0f;

	    // normala directiei daca se poate calcula
	    if (dist > 0.0001f) { nx = dirX / dist; ny = dirY / dist; }

	    //le pozitionez pe media overlapului ca sa nu mai fie una in alta.
	    float overlap = cellSize - dist;
	    s1.setPosition(Vector2f(p1.x - nx * overlap / 2.0f, p1.y - ny * overlap / 2.0f));
	    s2.setPosition(Vector2f(p2.x + nx * overlap / 2.0f, p2.y + ny * overlap / 2.0f));

	    // puterea de apropiere una spre alta
	    Vector2f v1 = s1.getSpeed(), v2 = s2.getSpeed();
	    float relVel = (v2.x - v1.x) * nx + (v2.y - v1.y) * ny;
	    if (relVel > 0) { return; }

		// aplic impulsul in sensul opus
	    float power = -1.99f * relVel / 2.0f;
	    s1.setSpeed(Vector2f(v1.x - power * nx, v1.y - power * ny));
	    s2.setSpeed(Vector2f(v2.x + power * nx, v2.y + power * ny));
	}
};