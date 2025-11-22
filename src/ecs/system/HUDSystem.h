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
    static constexpr float TITLE_SIZE = 25.0;
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

        auto& player1Health = player1Entity->getComponent<Health>();
        auto& player2Health = player2Entity->getComponent<Health>();

        auto& player1Yarnballs = player1Entity->getComponent<Yarnballs>();
        auto& player2Yarnballs = player2Entity->getComponent<Yarnballs>();

        for (auto& e : entities) {
            if (e->hasComponent<Label>()) {
                auto& label = e.get()->getComponent<Label>();
                std::stringstream ss;

                if (label.type == LabelType::PlayerTitle) {
                    if (label.textureCacheKey == "player1Title") {
                        ss << "Kitty 1";

                    } else if (label.textureCacheKey == "player2Title") {
                        ss << "Kitty 2";
                    }

                    label.text = ss.str();
                    label.dirty = true;
                }

                if (label.type == LabelType::Lives) {
                    if (label.textureCacheKey == "player1Lives") {
                        ss << "Lives: " << player1Health.currentHealth;

                    } else if (label.textureCacheKey == "player2Lives") {
                        ss << "Lives: " << player2Health.currentHealth;
                    }

                    label.text = ss.str();
                    label.dirty = true;
                }

                if (label.type == LabelType::Yarnballs) {
                    if (label.textureCacheKey == "player1Yarnballs") {
                        ss << "Yarnballs: " << player1Yarnballs.yarnballs;

                    } else if (label.textureCacheKey == "player2Yarnballs") {
                        ss << "Yarnballs: " << player2Yarnballs.yarnballs;
                    }

                    label.text = ss.str();
                    label.dirty = true;
                }

            }
        }


    };
};

#endif //INC_8552GAME_HUDSYSTEM_H