//
// Created by WillO on 2025-12-01.
//

#ifndef INC_8552GAME_INVINCIBILITYSYSTEM_H
#define INC_8552GAME_INVINCIBILITYSYSTEM_H
#include <memory>

#include "Component.h"

class InvincibilitySystem {
public:
    void update(std::vector<unique_ptr<Entity>>& entities, float deltaTime) {
        for (auto& entity : entities) {
            if (entity->hasComponent<Invincibility>()) {
                auto& invincibility = entity->getComponent<Invincibility>();
                invincibility.timeElapsedCurrent += deltaTime;
                invincibility.flashTimeElapsedCurrent += deltaTime;
                if (invincibility.flashTimeElapsedCurrent >= invincibility.flashInterval) {
                    invincibility.flashTimeElapsedCurrent = 0.0f;
                    invincibility.flash = !invincibility.flash;
                }
                if (invincibility.timeElapsedCurrent >= invincibility.duration) {
                    cout << "Removing invincibility" << endl;
                    entity->removeComponent<Invincibility>();
                }
            }
        }
    }
};

#endif //INC_8552GAME_INVINCIBILITYSYSTEM_H