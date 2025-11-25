//
// Created by WillO on 2025-11-24.
//

#ifndef INC_8552GAME_BALLTHROWSYSTEM_H
#define INC_8552GAME_BALLTHROWSYSTEM_H
#include <memory>

#include "Component.h"

class World;

class BallSystem {
    const int ballSpawnDistance = 70.0f;
public:

    void update(World& world, float dt);
};

#endif //INC_8552GAME_BALLTHROWSYSTEM_H