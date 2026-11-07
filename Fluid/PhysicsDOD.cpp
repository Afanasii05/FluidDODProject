#include "PhysicsDOD.h"
#include "PhysicsOOP.h"

PhysicsDOD::PhysicsDOD() {
    cellSize = 2 * globalVariables::circleRadius;
    rows = globalVariables::screenHeight / cellSize + 1;
    cols = globalVariables::screenWidth / cellSize + 1;
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
    cellSize = 2 * globalVariables::circleRadius;
    rows = globalVariables::screenHeight / cellSize + 1;
    cols = globalVariables::screenWidth / cellSize + 1;
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
    if (globalVariables::mergeEnabled) {
        float d = cellSize * (1.0f - globalVariables::mergeOverlap / 100.0f);
        mergeDistSqr = d * d;
    } else { mergeDistSqr = -1.0f; }

    updateGrid();
    updateCollidedCircles();
    removeDeadCircles();

    float dtmult = ImGui::GetIO().DeltaTime * globalVariables::speedMultiplier, radius = globalVariables::circleRadius;
    float max_x = globalVariables::screenWidth - radius, max_y = globalVariables::screenHeight - radius;

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