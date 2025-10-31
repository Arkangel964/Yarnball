//
// Created by WillO on 2025-10-21.
//

#ifndef TUTORIAL1_SPAWNTIMERSYSTEM_H
#define TUTORIAL1_SPAWNTIMERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

using namespace std;

class SpawnTimerSystem {
    public:
    void update(const vector<unique_ptr<Entity>>& entities, const float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<TimedSpawner>()) {
                auto& spawner = entity->getComponent<TimedSpawner>();
                spawner.timer -= dt;

                if (spawner.timer <= 0) {
                    spawner.timer = spawner.spawnInterval;
                    spawner.spawnCallback();
                }
            }
        }
    }
};


#endif //TUTORIAL1_SPAWNTIMERSYSTEM_H