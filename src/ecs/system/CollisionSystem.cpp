//
// Created by WillO on 2025-09-30.
//

#include "CollisionSystem.h"

#include <iostream>

#include "Collision.h"
#include "World.h"

void CollisionSystem::update(World& world) {
    //get a list of entities that have colliders and transforms
    const vector<Entity*> collidables = queryCollidables(world.getEntities());

    //update all collider positions first
    for (auto entity : collidables) {
        auto& t = entity->getComponent<Transform>();
        auto& c = entity->getComponent<Collider>();
        c.rect.x = t.position.x;
        c.rect.y = t.position.y;
    }

    std::set<CollisionKey> currentCollisions;

    //outer loop
    for (size_t i = 0; i < collidables.size(); i++) {
        //update the collider position
        auto entityA = collidables[i];
        auto& colA = entityA->getComponent<Collider>();
        if (!colA.enabled) {
            continue;
        }

        //check the collider collision
        //inner loop
        for (size_t j = i+1; j < collidables.size(); j++) {
            auto entityB = collidables[j];
            auto& colB = entityB->getComponent<Collider>();
            if (!colB.enabled) {
                continue;
            }
            if (Collision::AABB(colA, colB)) {
                //std::cout << colA.tag << " hit " << colB.tag << std::endl;
                CollisionKey key = makeKey(entityA, entityB);
                currentCollisions.insert(key);
                if (!activeCollisions.contains(key)) {
                    world.getEventManager().emit(CollisionEvent{entityA, entityB, CollisionState::Enter});
                    activeCollisions.insert(key);
                }
                world.getEventManager().emit(CollisionEvent{entityA, entityB, CollisionState::Stay});
            }
        }
    }

    for (auto& key : activeCollisions) {
        if (!currentCollisions.contains(key)) {
            world.getEventManager().emit(CollisionEvent{key.first, key.second, CollisionState::Exit});
        }
    }

    activeCollisions = std::move(currentCollisions); //update with current collisions
}

vector<Entity*> CollisionSystem::queryCollidables(const vector<unique_ptr<Entity>>& entities) {
    vector<Entity*> collidables;
    for (auto &entity : entities) {
        if (entity->hasComponent<Transform>() && entity->hasComponent<Collider>()) {
            collidables.push_back(entity.get());
        }
    }
    return collidables;
}
