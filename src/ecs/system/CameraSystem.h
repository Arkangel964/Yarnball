//
// Created by WillO on 2025-10-14.
//

#ifndef TUTORIAL1_CAMERASYSTEM_H
#define TUTORIAL1_CAMERASYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

using namespace std;

class CameraSystem {
    public:
    void update(const vector<unique_ptr<Entity>>& entities) {
        Entity* playerEntity = nullptr;

        //find player
        for (auto& e : entities) {
            if (e->hasComponent<Player1Tag>()) {
                playerEntity = e.get();
                break;
            }
        }

        if (!playerEntity) return;

        auto& playerTransform = playerEntity->getComponent<Transform>();

        for (auto& e : entities) {
            if (e->hasComponent<Camera>()) {
                auto& cam = e->getComponent<Camera>();
                cam.view.x = playerTransform.position.x - cam.view.w / 2;
                cam.view.y = playerTransform.position.y - cam.view.h / 2;

                //clamping
                if (cam.view.x < 0) cam.view.x = 0;
                if (cam.view.y < 0) cam.view.y = 0;
                if (cam.view.x > cam.worldWidth - cam.view.w) cam.view.x = cam.worldWidth - cam.view.w;
                if (cam.view.y > cam.worldHeight - cam.view.h) cam.view.y = cam.worldHeight - cam.view.h;


            }
        }
    }
};

#endif //TUTORIAL1_CAMERASYSTEM_H