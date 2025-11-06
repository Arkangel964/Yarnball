//
// Created by susan on 2025-11-05.
//

#ifndef INC_8552GAME_UIRENDERSYSTEM_H
#define INC_8552GAME_UIRENDERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "RenderUtils.h"
#include "TextureManager.h"

class UIRenderSystem {
    public:
    void render(const std::vector<std::unique_ptr<Entity>>& entities) {
        for (auto& e : entities) {
            if (e->hasComponent<Transform>() && e->hasComponent<Sprite>()) {
                auto transform = e->getComponent<Transform>();
                auto sprite = e->getComponent<Sprite>();

                if (sprite.renderLayer != RenderLayer::UI) continue;

                // no converting from world space to screen space
                sprite.dst.x = transform.position.x;
                sprite.dst.y = transform.position.y;

                if (sprite.visible) {
                    SDL_FRect scaledDest = RenderUtils::getScaledDest(sprite.dst, transform.scale);
                    TextureManager::draw(sprite.texture, sprite.src, scaledDest);
                }

            }
        }
    }
};

#endif //INC_8552GAME_UIRENDERSYSTEM_H