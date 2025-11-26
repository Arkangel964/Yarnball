//
// Created by Lukar on 04/11/2025.
//

#ifndef INC_8552GAME_PHYSICSSYSTEM_H
#define INC_8552GAME_PHYSICSSYSTEM_H


#include <vector>
#include <memory>
#include "Entity.h"
#include "Component.h"

class PhysicsSystem {
public:
    void update(std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            if (!entity->hasComponent<RigidBody>())
                continue;
            auto& rigidBody = entity->getComponent<RigidBody>();

            if (!entity->hasComponent<Impulse>()) {
                // No new impulse, decay normally
                if (!entity->hasComponent<Velocity>())
                    continue;
                auto& velocity = entity->getComponent<Velocity>();
                if (velocity.speed > 0.0f) {
                    velocity.speed = max(0.0f, velocity.speed - rigidBody.friction);
                    velocity.speed = velocity.speed;
                } else {
                    if (entity->hasComponent<DestroyOnStop>()) {
                        auto& callbackComponent = entity->getComponent<DestroyOnStop>();
                        if (callbackComponent.onDestroy) {
                            callbackComponent.onDestroy();
                        }
                        entity->destroy();
                    } else {
                        velocity.direction = {0, 0};
                    }
                }
            } else {
                auto& impulse = entity->getComponent<Impulse>();
                // Verify this rigidbody entity has a velocity
                if (!entity->hasComponent<Velocity>()) {
                    // Set direction and speed based on the incoming impulse
                    float speed = min(impulse.force, rigidBody.maxSpeed);    // Cap speed to maximum allowed value
                    entity->addComponent<Velocity>(impulse.direction, impulse.direction, speed);
                    // Remove impulse
                    entity->removeComponent<Impulse>();
                    continue;
                }
                // Get this entities velocity and merge with impulse
                auto& velocity = entity->getComponent<Velocity>();
                velocity.oldDirection = velocity.direction;

                Vector2D velocityVec = velocity.direction * velocity.speed;
                Vector2D impulseVec = impulse.direction * impulse.force;
                Vector2D resultVec = velocityVec + impulseVec;
                float newSpeed = min(resultVec.length(), rigidBody.maxSpeed);
                resultVec.normalize();
                velocity.direction = resultVec;
                velocity.speed = newSpeed;
                entity->removeComponent<Impulse>();
            }
        }
    }

    static void addImpulse(Entity &entity, Vector2D direction, float force) {
        if (force < 0) {
            direction *= -1;
            force = abs(force);
        }
        direction = direction.normalize();
        entity.addComponent<Impulse>(direction, force);
    }
};


#endif //INC_8552GAME_PHYSICSSYSTEM_H
