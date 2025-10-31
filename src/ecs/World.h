//
// Created by WillO on 2025-09-16.
//

#ifndef TUTORIAL1_WORLD_H
#define TUTORIAL1_WORLD_H
#include <memory>
#include <vector>

#include "AnimationSystem.h"
#include "CameraSystem.h"
#include "Map.h"
#include "CollisionSystem.h"
#include "DestructionSystem.h"
#include "Entity.h"
#include "EventResponseSystem.h"
#include "event/EventManager.h"
#include "KeyboardInputSystem.h"
#include "MainMenuSystem.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "SpawnTimerSystem.h"
#include "scene/SceneType.h"

class World {
    Map map;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Entity>> deferredEntities;
    MovementSystem movementSystem;
    RenderSystem renderSystem;
    KeyboardInputSystem keyboardInputSystem;
    AnimationSystem animationSystem;
    CollisionSystem collisionSystem;
    CameraSystem cameraSystem;
    EventManager eventManager;
    SpawnTimerSystem spawnTimerSystem;
    DestructionSystem destructionSystem;
    EventResponseSystem eventResponseSystem{*this};
    MainMenuSystem mainMenuSystem;
public:
    World() = default;

    void update(float deltaTime, const SDL_Event &event, SceneType sceneType) {
        if (sceneType == SceneType::MainMenu) {
            mainMenuSystem.update(event);
        } else {
            keyboardInputSystem.update(entities, event);
            movementSystem.update(entities, deltaTime);
            collisionSystem.update(*this);
            animationSystem.update(entities, deltaTime);
            cameraSystem.update(entities);
            spawnTimerSystem.update(entities, deltaTime);
            destructionSystem.update(entities);
        }
        synchronizeEntities();
        cleanup();
    };

    void render() {
        for (auto& entity : entities) {
            if (entity->hasComponent<Camera>()) {
                map.draw(entity->getComponent<Camera>());
                break;
            }
        }
        renderSystem.render(entities);
    };

    Entity& createEntity() {
        //we use emplace instead of push so we don't create a copy
        entities.emplace_back(std::make_unique<Entity>());
        return *entities.back();
    };

    Entity& createDeferredEntity() {
        deferredEntities.emplace_back(std::make_unique<Entity>());
        return *deferredEntities.back();
    }

    std::vector<std::unique_ptr<Entity>>& getEntities() {
        return entities;
    };

    void cleanup() {
        //use a lambda predicate to remove inactive entities
        std::erase_if(entities, [](std::unique_ptr<Entity>& e) {
            return !e->isActive();
        });
    };

    void synchronizeEntities() {
        if (!deferredEntities.empty()) {
            //push back all deferred entities to the entities vector
            //using move to not create a copy (so we maintain the same reference later)
            std::move(deferredEntities.begin(), deferredEntities.end(), std::back_inserter(entities));

            //clearing the creation buffer
            deferredEntities.clear();
        }
    }

    EventManager& getEventManager() {
        return eventManager;
    }

    Map& getMap() {
        return map;
    }
};

#endif //TUTORIAL1_WORLD_H