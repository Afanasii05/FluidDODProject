#pragma once
#include "Shader.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include "ourMath.h"
#include "globalVariables.h"
#include "imgui.h"
using namespace ourMath;

class PhysicsOOP;

class PhysicsDOD
{
    std::vector<std::vector<int>> grid;
    std::vector<Vector2f> posC;
    std::vector<Vector2f> speedC;
    float cellSize, squaredAddedCirclesRadius;
    int cols, rows;
    GLuint Vao, Vbo;

public:
    PhysicsDOD();
    PhysicsDOD(const PhysicsOOP& oop);
    ~PhysicsDOD();

    [[nodiscard]] const std::vector<std::vector<int>>& getGrid() const { return grid; }
    [[nodiscard]] const std::vector<Vector2f>& getPositions() const { return posC; }
    [[nodiscard]] const std::vector<Vector2f>& getSpeeds() const { return speedC; }
    [[nodiscard]] float getCellSize() const { return cellSize; }
    [[nodiscard]] int getCols() const { return cols; }
    [[nodiscard]] int getRows() const { return rows; }
    [[nodiscard]] float getSquaredRadius() const { return squaredAddedCirclesRadius; }
    [[nodiscard]] GLuint getVAO() const { return Vao; }
    [[nodiscard]] GLuint getVBO() const { return Vbo; }

    void setGrid(const std::vector<std::vector<int>>& grd) { this->grid = grd; }
    void setPositions(const std::vector<Vector2f>& pos) { this->posC = pos; }
    void setSpeeds(const std::vector<Vector2f>& spd) { this->speedC = spd; }
    void setCellSize(float cell_size) { cellSize = cell_size; }
    void setCols(int cls) { this->cols = cls; }
    void setRows(int rws) { this->rows = rws; }
    void setSquaredRadius(float sqr_rad) { squaredAddedCirclesRadius = sqr_rad; }
    void setVAO(GLuint vao) { Vao = vao; }
    void setVBO(GLuint vbo) { Vbo = vbo; }



    // .cpp functions
    void render(Shader& sh);
    void update();
    void updateGrid();
    void changeNumberOfCircles(int number);



    // inline functions
    int cellIndex(Vector2f position) const {
        int x = std::clamp(int(position.x / cellSize), 0, cols - 1);
        int y = std::clamp(int(position.y / cellSize), 0, rows - 1);
        return x + y * cols;
    }

    void checkBetweenCells(const std::vector<int>& cell, const int x, const int y) {
        if (x < 0 || y < 0 || x >= cols || y >= rows)
            return;
        const std::vector<int>& neighbor = grid[x + cols * y];

        for (const int a : cell)
            for (const int b : neighbor)
                resolveColision(a, b);
    }

    void updateCollidedCircles() {
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                const std::vector<int>& cell = grid[x + cols * y];
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

    void resolveColision(const int idx_c1, const int idx_c2) {
        Vector2f p1 = posC[idx_c1], p2 = posC[idx_c2];
        float dirX = p2.x - p1.x, dirY = p2.y - p1.y;
        float dist_sqr = dirX * dirX + dirY * dirY;

        // verifica daca cele doua cercuri sunt in coliziune
        if (dist_sqr >= squaredAddedCirclesRadius) { return; }

        // distanta necesara a.i. sa nu mai colizioneze
        float dist = std::sqrt(dist_sqr);
        float nx = 1.0f, ny = 0.0f;

        // normala directiei daca se poate calcula
        if (dist > 0.0001f) { nx = dirX / dist; ny = dirY / dist; }

        // mutam cat jumate din overlap fiindca ambele cercuri se misca odata
        float overlap = cellSize - dist;
        posC[idx_c1] = Vector2f(p1.x - nx * overlap / 2.0f, p1.y - ny * overlap / 2.0f);
        posC[idx_c2] = Vector2f(p2.x + nx * overlap / 2.0f, p2.y + ny * overlap / 2.0f);

        // puterea de apropiere una spre alta
        float relVel = (speedC[idx_c2].x - speedC[idx_c1].x) * nx + (speedC[idx_c2].y - speedC[idx_c1].y) * ny;
        if (relVel > 0) { return; }

        // aplic impulsul in sensul opus
        float power = -1.99f * relVel / 2.0f;
        speedC[idx_c1] = Vector2f(speedC[idx_c1].x - power * nx, speedC[idx_c1].y - power * ny);
        speedC[idx_c2] = Vector2f(speedC[idx_c2].x + power * nx, speedC[idx_c2].y + power * ny);
    }
};