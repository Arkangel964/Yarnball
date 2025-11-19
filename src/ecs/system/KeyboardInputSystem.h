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
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const std::vector<SDL_Event>& events) {
        static Vector2D player1Input(0, 0);
        static Vector2D player2Input(0, 0);
        for (auto& entity : entities) {
            if (entity->hasComponent<Player1Tag>() && entity->hasComponent<RigidBody>()) {
                for (const auto& event: events) {
                    if (event.type == SDL_EVENT_KEY_DOWN) {
                        switch (event.key.key) {
                        case SDLK_W:
                            player1Input.x = 0;
                            player1Input.y = -1;
                            break;
                        case SDLK_S:
                            player1Input.x = 0;
                            player1Input.y = 1;
                            break;
                        case SDLK_A:
                            player1Input.x = -1;
                            player1Input.y = 0;
                            break;
                        case SDLK_D:
                            player1Input.x = 1;
                            player1Input.y = 0;
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
                            player1Input.x = 0;
                            player1Input.y = 0;
                            break;
                        default:
                            break;
                        }
                    }
                }
                if (player1Input != Vector2D())
                    PhysicsSystem::addImpulse(*entity, player1Input, 240);
            } else if (entity->hasComponent<Player2Tag>() && entity->hasComponent<RigidBody>()) {
                for (const auto& event: events) {
                    if (event.type == SDL_EVENT_KEY_DOWN) {
                        switch (event.key.key) {
                        case SDLK_UP:
                            player2Input.x = 0;
                            player2Input.y = -1;
                            break;
                        case SDLK_DOWN:
                            player2Input.x = 0;
                            player2Input.y = 1;
                            break;
                        case SDLK_LEFT:
                            player2Input.x = -1;
                            player2Input.y = 0;
                            break;
                        case SDLK_RIGHT:
                            player2Input.x = 1;
                            player2Input.y = 0;
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
                            player2Input.x = 0;
                            player2Input.y = 0;
                            break;
                        default:
                            break;
                        }
                    }
                }
                if (player2Input != Vector2D())
                    PhysicsSystem::addImpulse(*entity, player2Input, 240);
            }
        }
    }
};

#endif //TUTORIAL1_KEYBOARDINPUTSYSTEM_H