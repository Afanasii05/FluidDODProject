#include "PhysicsOOP.h"
#include "PhysicsDOD.h"

PhysicsOOP::PhysicsOOP() { //setez ca marimea unui grid sa fie cat diametrul unei bile(bilele trebuie sa aibe toate aceeasi marime)
    cellSize =  2 * globalVariables::circleRadius;
    rows = globalVariables::screenHeight / cellSize + 1;
    cols = globalVariables::screenWidth / cellSize + 1;
    squaredAddedCirclesRadius = cellSize * cellSize;
    grid.assign(rows *cols, {});

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

PhysicsOOP::PhysicsOOP(const PhysicsDOD& dod) {
    cellSize = 2 * globalVariables::circleRadius;
    rows = globalVariables::screenHeight / cellSize + 1;
    cols = globalVariables::screenWidth / cellSize + 1;
    squaredAddedCirclesRadius = cellSize * cellSize;

    // copy between the two
    grid.assign(cols * rows, {});
    shapes.reserve(dod.getPositions().size());

    std::transform(
        dod.getPositions().begin(), dod.getPositions().end(),
        dod.getSpeeds().begin(),
        std::back_inserter(shapes),
        [](const Vector2f& pos, const Vector2f& spd) -> movableShape* {
            return new Circle(pos, spd, {0, 0}, globalVariables::circleRadius);
        }
    );

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

PhysicsOOP::~PhysicsOOP() {
    for (movableShape* shape : shapes)
        delete shape;

    glDeleteBuffers(1, &VboPos);
    glDeleteBuffers(1, &VboSpd);
    glDeleteVertexArrays(1, &Vao);
}

void PhysicsOOP::render(Shader& sh) {
    std::vector<Vector2f> positions; positions.reserve(shapes.size());
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(positions), [](movableShape* shape){ return shape->getPosition(); });

    std::vector<Vector2f> speeds; speeds.reserve(shapes.size());
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(speeds), [](movableShape* shape){ return shape->getSpeed(); });

    glBindVertexArray(Vao);

    glBindBuffer(GL_ARRAY_BUFFER, VboPos);
    glBufferData(GL_ARRAY_BUFFER, shapes.size() * sizeof(Vector2f), positions.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VboSpd);
    glBufferData(GL_ARRAY_BUFFER, shapes.size() * sizeof(Vector2f), speeds.data(), GL_STREAM_DRAW);

    glDrawArrays(GL_POINTS, 0, (GLsizei)shapes.size());
    glBindVertexArray(0);
}

void PhysicsOOP::update() {
    updateGrid();
    updateCollidedCircles();

    float dtmult = ImGui::GetIO().DeltaTime * globalVariables::speedMultiplier, radius = globalVariables::circleRadius;
    float max_x = globalVariables::screenWidth - radius, max_y = globalVariables::screenHeight - radius;

    for (movableShape* shape : shapes) {
        Vector2f pos = shape->getPosition(), speed = shape->getSpeed();
        pos.x += speed.x * dtmult; pos.y += speed.y * dtmult;

        if (pos.x <= radius) {
            pos.x = radius; if (speed.x < 0) { speed.x = -speed.x; }
        } else if (pos.x >= max_x) {
            pos.x = max_x; if (speed.x > 0) { speed.x = -speed.x; }
        }

        if (pos.y <= radius) {
            pos.y = radius; if (speed.y < 0) { speed.y = -speed.y; }
        } else if (pos.y >= max_y) {
            pos.y = max_y; if (speed.y > 0) { speed.y = -speed.y; }
        }

        shape->setPosition(pos);
        shape->setSpeed(speed);
    }
}

void PhysicsOOP::updateGrid() {
    for (auto& gridCell : grid) {
        gridCell.clear();
    }

    int shape_nr = shapes.size();
    for (int i = 0; i < shape_nr; i++) {
        grid[cellIndex(shapes[i]->getPosition())].push_back(i);
    }
}

void PhysicsOOP::addCircles(std::vector<movableShape*> addedShapes) {
    shapes.insert(shapes.end(), addedShapes.begin(), addedShapes.end());
}

void PhysicsOOP::addCircle(Circle* cerc) {
    shapes.push_back(cerc);
}

void PhysicsOOP::changeNumberOfCircles(int number) {
    if (number > shapes.size()) {
        for (int i = 0;i < number - shapes.size();i++) {
            addCircle(new Circle(
                { randomFloat(200,1700), randomFloat(200,800) },
                { randomFloat(-20, 20), randomFloat(-20, 20) },
                {0,0},
                globalVariables::circleRadius
            ));
        }
    }
    else {
        for (int i = shapes.size() - 1; i >= number;i--) {
            delete shapes[i];
            shapes.pop_back();
        }
    }
}