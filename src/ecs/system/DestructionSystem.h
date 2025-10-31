//
// Created by WillO on 2025-10-21.
//

#ifndef TUTORIAL1_DESTRUCTIONSYSTEM_H
#define TUTORIAL1_DESTRUCTIONSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

using namespace std;

class DestructionSystem {
    public:
    void update(const vector<unique_ptr<Entity>>& entities) {
        Entity* cameraEntity = nullptr;

        for (auto& e : entities) {
            if (e->hasComponent<Camera>()) {
                cameraEntity = e.get();
                break;
            }
        }

        if (!cameraEntity) return;
        auto& cam = cameraEntity->getComponent<Camera>();

        for (auto& e : entities) {
            if (e->hasComponent<Transform>() && e->hasComponent<ProjectileTag>()) {
                auto& t = e->getComponent<Transform>();
                if (t.position.x > cam.view.x + cam.view.w ||
                    t.position.x < cam.view.x ||
                    t.position.y > cam.view.y + cam.view.h ||
                    t.position.y < cam.view.y) {

                    e->destroy();
                }
            }
        }
    }



};

#endif //TUTORIAL1_DESTRUCTIONSYSTEM_H