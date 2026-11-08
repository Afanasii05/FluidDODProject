#include "PhysicsDOD.h"
#include "PhysicsOOP.h"

PhysicsDOD::PhysicsDOD() {
    cellSize = 2 * global::circleRadius;
    rows = global::screenHeight / cellSize + 1;
    cols = global::screenWidth / cellSize + 1;
    squaredAddedCirclesRadius = cellSize * cellSize;
    grid.assign(cols * rows, {});

    //Generare de buffere pentru rendare
    glGenVertexArrays(1, &Vao);
    glGenBuffers(1, &VboPos);
    glGenBuffers(1, &VboSpd);

    glBindVertexArray(Vao);

    glBindBuffer(GL_ARRAY_BUFFER, VboPos);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VboSpd);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

PhysicsDOD::~PhysicsDOD() {
    glDeleteBuffers(1, &VboSpd);
    glDeleteBuffers(1, &VboPos);
    glDeleteVertexArrays(1, &Vao);
}

PhysicsDOD::PhysicsDOD(const PhysicsOOP& oop) {
    cellSize = 2 * global::circleRadius;
    rows = global::screenHeight / cellSize + 1;
    cols = global::screenWidth / cellSize + 1;
    squaredAddedCirclesRadius = cellSize * cellSize;

    // copy between the two
    grid.assign(cols * rows, {});
    const auto& shapes = oop.getShapes();
    posC.reserve(shapes.size()); speedC.reserve(shapes.size());

    std::transform(shapes.begin(), shapes.end(), std::back_inserter(posC), [](movableShape* shape){ return shape->getPosition(); });
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(speedC), [](movableShape* shape){ return shape->getSpeed(); });

    //Generare de buffere pentru rendare
    glGenVertexArrays(1, &Vao);
    glGenBuffers(1, &VboPos);
    glGenBuffers(1, &VboSpd);

    glBindVertexArray(Vao);

    glBindBuffer(GL_ARRAY_BUFFER, VboPos);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VboSpd);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f), (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void PhysicsDOD::render(Shader& sh) {
    sh.use();
    glBindVertexArray(Vao);

    glBindBuffer(GL_ARRAY_BUFFER, VboPos);
    glBufferData(GL_ARRAY_BUFFER, posC.size() * sizeof(Vector2f), posC.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VboSpd);
    glBufferData(GL_ARRAY_BUFFER, speedC.size() * sizeof(Vector2f), speedC.data(), GL_STREAM_DRAW);

    glDrawArrays(GL_POINTS, 0, (GLsizei)posC.size());
    glBindVertexArray(0);
}

void PhysicsDOD::update() {
    dead.assign(posC.size(), 0); deadCount = 0;
    if (global::mergeEnabled) {
        float d = cellSize * (1.0f - global::mergeOverlap / 100.0f);
        mergeDistSqr = d * d;
    } else { mergeDistSqr = -1.0f; }

    updateGrid();
    updateCollidedCircles();
    updateCursorRepulsion();
    removeDeadCircles();

    
    float dtmult = ImGui::GetIO().DeltaTime * global::speedMultiplier, radius = global::circleRadius;
    float max_x = global::screenWidth - radius, max_y = global::screenHeight - radius;

    for (int i = 0;i < posC.size();i++) {
        if (posC[i].x <= radius) {
            posC[i].x = radius; if (speedC[i].x < 0) { speedC[i].x = -speedC[i].x; }
        } else if (posC[i].x >= max_x) {
            posC[i].x = max_x; if (speedC[i].x > 0) { speedC[i].x = -speedC[i].x; }
        }

        if (posC[i].y <= radius) {
            posC[i].y = radius; if (speedC[i].y < 0) { speedC[i].y = -speedC[i].y; }
        } else if (posC[i].y >= max_y) {
            posC[i].y = max_y; if (speedC[i].y > 0) { speedC[i].y = -speedC[i].y; }
        }

        posC[i] = posC[i] + speedC[i] * dtmult;
    }
}

void PhysicsDOD::updateCursorRepulsion() {
    if (!global::cursorRepelEnabled) return;

    Vector2f mousePos(global::mouseX, global::mouseY);
    float repelRadius = global::cursorRepelRadius;
    float repelRadiusSqr = repelRadius * repelRadius;


    int mouseCellX = std::clamp(int(mousePos.x / cellSize), 0, cols - 1);
    int mouseCellY = std::clamp(int(mousePos.y / cellSize), 0, rows - 1);

    // Range-ul de celule verificate
    int range = std::max(1, (int)(repelRadius / cellSize));
    
    for (int y = std::max(0, mouseCellY - range); y <= std::min(rows - 1, mouseCellY + range); y++) {
        for (int x = std::max(0, mouseCellX - range); x <= std::min(cols - 1, mouseCellX + range); x++) {
            const std::vector<int>& cell = grid[x + cols * y];
            for (int idx : cell) {

                Vector2f p = posC[idx];
                float dirX = p.x - mousePos.x;
                float dirY = p.y - mousePos.y;
                float distSqr = dirX * dirX + dirY * dirY;

                if (distSqr < repelRadiusSqr && distSqr > 0.0001f) {
                    float dist = std::sqrt(distSqr);
                    float nx = dirX / dist;
                    float ny = dirY / dist;

                    float factor = (1.0f - (dist / repelRadius));
                    speedC[idx].x += nx * factor * global::repelForce; //Puterea fortei
                    speedC[idx].y += ny * factor * global::repelForce;

                }
            }
        }
    }
}

void PhysicsDOD::removeDeadCircles() {
    if (deadCount == 0) return;

    int j = 0;
    for (int i = 0; i < posC.size(); i++) {
        if (dead[i]) continue;
        posC[j] = posC[i];
        speedC[j] = speedC[i];
        j++;
    }
    posC.resize(j);
    speedC.resize(j);
}

void PhysicsDOD::updateGrid() {
    for (auto& gridCell : grid) {
        gridCell.clear();
    }
    for (int i = 0;i < posC.size();i++) {
        grid[cellIndex(posC[i])].push_back(i);
    }
}

void PhysicsDOD::changeNumberOfCircles(int number) {
    if (number > posC.size()) {
        while (posC.size() < number) {
            posC.push_back({ ourMath::randomFloat(200, 1700), ourMath::randomFloat(200, 800) });
            speedC.push_back({ ourMath::randomFloat(-20, 20), ourMath::randomFloat(-20, 20) });
        }
    }
    else {
        while (posC.size() > number) {
            posC.pop_back();
            speedC.pop_back();
        }
    }
}