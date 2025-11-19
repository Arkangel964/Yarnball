//
// Created by WillO on 2025-09-23.
//

#ifndef TUTORIAL1_KEYBOARDINPUTSYSTEM_H
#define TUTORIAL1_KEYBOARDINPUTSYSTEM_H
#include <iostream>
#include <memory>
#include <vector>
#include <SDL3/SDL_events.h>

#include "Component.h"
#include "Entity.h"
#include "PhysicsSystem.h"

class KeyboardInputSystem {
private:
    std::vector<SDL_Keycode> pressedKeys;

public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, const std::vector<SDL_Event>& events) {
        for (const auto& event: events) {
            if (event.type == SDL_EVENT_KEY_DOWN) {
                auto it = std::find(pressedKeys.begin(), pressedKeys.end(), event.key.key);
                if (it == pressedKeys.end()) {
                    pressedKeys.push_back(event.key.key);
                }
            } else if (event.type == SDL_EVENT_KEY_UP) {
                auto it = std::find(pressedKeys.begin(), pressedKeys.end(), event.key.key);
                if (it != pressedKeys.end()) {
                    pressedKeys.erase(it);
                }
            }
        }

        std::cout << "Pressed Keys: ";
        for (const auto& key: pressedKeys) {
            std::cout << key << " ";
        }
        std::cout << std::endl;

        for (auto& entity : entities) {
            if (entity->hasComponent<Keybinds>() && entity->hasComponent<RigidBody>()) {
                auto& keybinds = entity->getComponent<Keybinds>();
                Vector2D playerInput{0, 0};
                for (const auto& key: pressedKeys) {
                    if (key == keybinds.up) {
                        playerInput.y = -1;
                    } else if (key == keybinds.down) {
                        playerInput.y = 1;
                    } else if (key == keybinds.left) {
                        playerInput.x = -1;
                    } else if (key == keybinds.right) {
                        playerInput.x = 1;
                    }
                }

                if (playerInput != Vector2D())
                    PhysicsSystem::addImpulse(*entity, playerInput, 240);
            }
        }
    }
};

#endif //TUTORIAL1_KEYBOARDINPUTSYSTEM_H