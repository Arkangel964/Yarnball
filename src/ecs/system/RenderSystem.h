//
// Created by WillO on 2025-09-16.
//

#ifndef TUTORIAL1_RENDERSYSTEM_H
#define TUTORIAL1_RENDERSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"
#include "TextureManager.h"

class RenderSystem {
public:
    void render(std::vector<std::unique_ptr<Entity>>& entities) {
        Entity* cameraEntity = nullptr;
        for (auto& e : entities) {
            if (e->hasComponent<Camera>()) {
                cameraEntity = e.get();
                break;
            }
        }

        if (!cameraEntity) return;
        auto& camera = cameraEntity->getComponent<Camera>();

        for (auto& entity : entities) {
            if (entity->isActive() && entity->hasComponent<Transform>() && entity->hasComponent<Sprite>()) {
                auto& transform = entity->getComponent<Transform>();
                auto& sprite = entity->getComponent<Sprite>();

                sprite.dst.x = transform.position.x - camera.view.x;
                sprite.dst.y = transform.position.y - camera.view.y;

                if (sprite.renderLayer != RenderLayer::World) continue; // filter out UI

                //if the entity has animation, update the src rect
                if (entity->hasComponent<Animation>()) {
                    auto& anim = entity->getComponent<Animation>();
                    sprite.src = anim.clips[anim.currentClip].frameIndices[anim.currentFrame];
                }

                TextureManager::draw(sprite.texture, sprite.src, sprite.dst);
            }
        }
    };
};

#endif //TUTORIAL1_RENDERSYSTEM_H