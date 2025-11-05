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
#include "PhysicsSystem.h"

class KeyboardInputSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const SDL_Event& event) {
        static Vector2D *currentInput = nullptr;
        for (auto& entity : entities) {
            if (entity->hasComponent<PlayerTag>() && entity->hasComponent<RigidBody>()) {
                if (event.type == SDL_EVENT_KEY_DOWN) {
                    switch (event.key.key) {
                        case SDLK_W:
                            currentInput = new Vector2D(0, -1);
                            break;
                        case SDLK_S:
                            currentInput = new Vector2D(0, 1);
                            break;
                        case SDLK_A:
                            currentInput = new Vector2D(-1, 0);
                            break;
                        case SDLK_D:
                            currentInput = new Vector2D(1, 0);
                            break;
                        default:
                            break;
                    };
                };

                if (event.type == SDL_EVENT_KEY_UP) {
                    switch (event.key.key) {
                        case SDLK_W:
                        case SDLK_S:
                        case SDLK_A:
                        case SDLK_D:
                            delete currentInput;
                            currentInput = nullptr;
                            break;
                        default:
                            break;
                    };
                };
            };
            if (currentInput)
                PhysicsSystem::addImpulse(*entity, *currentInput, 240);
        };
    };
};

#endif //TUTORIAL1_KEYBOARDINPUTSYSTEM_H