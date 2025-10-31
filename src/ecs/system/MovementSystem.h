//
// Created by WillO on 2025-09-16.
//

#ifndef TUTORIAL1_MOVEMENTSYSTEM_H
#define TUTORIAL1_MOVEMENTSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

class MovementSystem {
    public:
        void update(std::vector<std::unique_ptr<Entity>>& entities, float deltaTime) {
            for (auto& entity : entities) {
                if (entity->hasComponent<Transform>() && entity->hasComponent<Velocity>()) {
                    auto& transform = entity->getComponent<Transform>();
                    auto& v = entity->getComponent<Velocity>();

                    //track previous frame's position
                    transform.oldPosition = transform.position;

                    Vector2D directionVec = v.direction;
                    directionVec.normalize();
                    //Vector2D needs an operator function to multiply a float to itself
                    Vector2D velocityVec = directionVec * v.speed;

                    transform.position += {velocityVec * deltaTime};

                }
            }
        }
    };

#endif //TUTORIAL1_MOVEMENTSYSTEM_H