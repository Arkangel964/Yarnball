//
// Created by susan on 2025-11-18.
//

#ifndef INC_8552GAME_PRERENDERSYSTEM_H
#define INC_8552GAME_PRERENDERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"


class PreRenderSystem {
    public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& entity : entities) {
            if (entity->hasComponent<Label>() && entity->getComponent<Label>().dirty) {
                // update label texture

            }
        }
    }
};


#endif //INC_8552GAME_PRERENDERSYSTEM_H