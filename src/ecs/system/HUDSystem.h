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
    static constexpr float TITLE_SIZE = 20.0;
    static constexpr float FONT_SIZE = 20.0;
    public:
    static float getTitleSize() {
        return TITLE_SIZE;
    }
    static float getFontSize() {
        return FONT_SIZE;
    }
    void update(const std::vector<std::unique_ptr<Entity>>& entities) {

        Entity* player1Entity = nullptr;
        Entity* player2Entity = nullptr;

        // find player
        for (auto& e: entities) {
            if (player1Entity && player2Entity) {
                break;
            }
            if (e->hasComponent<Player1Tag>()) {
                player1Entity = e.get();
            }
            if (e->hasComponent<Player2Tag>()) {
                player2Entity = e.get();
            }
        }

        if (!player1Entity || !player2Entity) return;

        auto& player1Transform = player1Entity->getComponent<Transform>();
        auto& player2Transform = player2Entity->getComponent<Transform>();

        for (auto& e : entities) {
            if (e->hasComponent<Label>()) {
                auto& label = e.get()->getComponent<Label>();

                // update player position label
                // if (label.type == LabelType::PlayerPosition) {
                //     std::stringstream ss;
                //     if (label.textureCacheKey == "player1Pos") {
                //         ss << "Player 1 position: " << player1Transform.position;
                //     } else if (label.textureCacheKey == "player2Pos") {
                //         ss << "Player 2 position: " << player2Transform.position;
                //     }
                //     label.text = ss.str();
                //     // we want to have an alternative system to update texture after this point,
                //     // as this is quite expensive
                //     label.dirty = true;
                // }

                if (label.type == LabelType::PlayerTitle) {
                    std::cout << "here" << std::endl;
                    std::stringstream ss;
                    if (label.textureCacheKey == "player1Title") {
                        ss << "Kitty 1";

                    } else if (label.textureCacheKey == "player2Title") {
                        ss << "Kitty 2";
                    }

                    label.text = ss.str();
                    label.dirty = true;
                }

            }
        }


    };
};

#endif //INC_8552GAME_HUDSYSTEM_H