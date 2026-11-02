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
    void setSquaredRadius(int sqr_rad) { squaredAddedCirclesRadius = sqr_rad; }
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

    void resolveColision(const int indexC1, const int indexC2) {
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
};