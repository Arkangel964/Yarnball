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
        static Vector2D *player1Input = nullptr;
        static Vector2D *player2Input = nullptr;
        for (auto& entity : entities) {
            if (entity->hasComponent<Player1Tag>() && entity->hasComponent<RigidBody>()) {
                if (event.type == SDL_EVENT_KEY_DOWN) {
                    switch (event.key.key) {
                        case SDLK_W:
                            player1Input = new Vector2D(0, -1);
                            break;
                        case SDLK_S:
                            player1Input = new Vector2D(0, 1);
                            break;
                        case SDLK_A:
                            player1Input = new Vector2D(-1, 0);
                            break;
                        case SDLK_D:
                            player1Input = new Vector2D(1, 0);
                            break;
                        default:
                            break;
                    }
                }

                if (event.type == SDL_EVENT_KEY_UP) {
                    switch (event.key.key) {
                        case SDLK_W:
                        case SDLK_S:
                        case SDLK_A:
                        case SDLK_D:
                            delete player1Input;
                            player1Input = nullptr;
                            break;
                        default:
                            break;
                    }
                }
                if (player1Input)
                    PhysicsSystem::addImpulse(*entity, *player1Input, 240);
            } else if (entity->hasComponent<Player2Tag>() && entity->hasComponent<RigidBody>()) {
                if (event.type == SDL_EVENT_KEY_DOWN) {
                    switch (event.key.key) {
                        case SDLK_UP:
                            player2Input = new Vector2D(0, -1);
                            break;
                        case SDLK_DOWN:
                            player2Input = new Vector2D(0, 1);
                            break;
                        case SDLK_LEFT:
                            player2Input = new Vector2D(-1, 0);
                            break;
                        case SDLK_RIGHT:
                            player2Input = new Vector2D(1, 0);
                            break;
                        default:
                            break;
                    }
                }

                if (event.type == SDL_EVENT_KEY_UP) {
                    switch (event.key.key) {
                        case SDLK_UP:
                        case SDLK_DOWN:
                        case SDLK_LEFT:
                        case SDLK_RIGHT:
                            delete player2Input;
                            player2Input = nullptr;
                            break;
                        default:
                            break;
                    }
                }
                if (player2Input)
                    PhysicsSystem::addImpulse(*entity, *player2Input, 240);
            }
        }
    }
};

#endif //TUTORIAL1_KEYBOARDINPUTSYSTEM_H