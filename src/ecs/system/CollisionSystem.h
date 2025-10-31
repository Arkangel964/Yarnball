//
// Created by WillO on 2025-09-30.
//

#ifndef TUTORIAL1_COLLISIONSYSTEM_H
#define TUTORIAL1_COLLISIONSYSTEM_H
#include <algorithm>
#include <memory>
#include <set>
#include <vector>

#include "Entity.h"

using CollisionKey = std::pair<Entity*, Entity*>;

class World;
using namespace std;

class CollisionSystem {
public:
    void update(World& world);
    std::set<CollisionKey> activeCollisions;
private:
    vector<Entity*> queryCollidables(const vector<unique_ptr<Entity>>& entities);
    CollisionKey makeKey(Entity* entityA, Entity* entityB) {
        return std::minmax(entityA, entityB); //automatically orders our pair so smaller number (memory address) is ordered first
    }
};

#endif //TUTORIAL1_COLLISIONSYSTEM_H