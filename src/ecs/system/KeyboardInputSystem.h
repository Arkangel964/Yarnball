//
// Created by WillO on 2025-09-23.
//

#ifndef TUTORIAL1_KEYBOARDINPUTSYSTEM_H
#define TUTORIAL1_KEYBOARDINPUTSYSTEM_H
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Component.h"
#include "Entity.h"

class KeyboardInputSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event& event) {
        for (auto& entity : entities) {
            if (entity->hasComponent<Player1Tag>() && entity->hasComponent<Velocity>()) {
                auto& velocity = entity->getComponent<Velocity>();
                if (event.type == SDL_EVENT_KEY_DOWN) {
                    switch (event.key.key) {
                        case SDLK_W:
                            velocity.direction.y = -1;
                            velocity.oldDirection.y = -1;
                            velocity.oldDirection.x = 0;
                            break;
                        case SDLK_S:
                            velocity.direction.y = 1;
                            velocity.oldDirection.y = 1;
                            velocity.oldDirection.x = 0;
                            break;
                        case SDLK_A:
                            velocity.direction.x = -1;
                            velocity.oldDirection.x = -1;
                            velocity.oldDirection.y = 0;
                            break;
                        case SDLK_D:
                            velocity.direction.x = 1;
                            velocity.oldDirection.x = 1;
                            velocity.oldDirection.y = 0;
                            break;
                        default:
                            break;
                    };
                };

                if (event.type == SDL_EVENT_KEY_UP) {
                    switch (event.key.key) {
                        case SDLK_W:
                            velocity.direction.y = 0;
                            break;
                        case SDLK_S:
                            velocity.direction.y = 0;
                            break;
                        case SDLK_A:
                            velocity.direction.x = 0;
                            break;
                        case SDLK_D:
                            velocity.direction.x = 0;
                            break;
                        default:
                            break;
                    };
                };
            };
        };
    };
};

#endif //TUTORIAL1_KEYBOARDINPUTSYSTEM_H