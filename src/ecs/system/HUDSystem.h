//
// Created by susan on 2025-11-18.
//

#ifndef INC_8552GAME_HUDSYSTEM_H
#define INC_8552GAME_HUDSYSTEM_H
#include <memory>
#include <sstream>
#include <vector>

#include "Component.h"
#include "Entity.h"

class HUDSystem {
    public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities) {

        Entity* player1Entity = nullptr;

        // find player
        for (auto& e: entities) {
            if (e->hasComponent<Player1Tag>()) {
                player1Entity = e.get();
                break;
            }
        }

        if (!player1Entity) return;

        auto& playerTransform = player1Entity->getComponent<Transform>();

        for (auto& e : entities) {
            if (e->hasComponent<Label>()) {
                auto& label = e.get()->getComponent<Label>();

                // update player position label
                if (label.type == LabelType::PlayerPosition) {
                    std::stringstream ss;
                    ss << "Player position: " << playerTransform.position;
                    label.text = ss.str();
                    // we want to have an alternative system to update texture after this point,
                    // as this is quite expensive
                    label.dirty = true;
                }

            }
        }


    };
};

#endif //INC_8552GAME_HUDSYSTEM_H