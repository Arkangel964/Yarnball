//
// Created by WillO on 2025-10-30.
//
#include "EventResponseSystem.h"

#include "Game.h"
#include "World.h"

EventResponseSystem::EventResponseSystem(World &world) {
    //subscriptions
    world.getEventManager().subscribe([this, &world](const BaseEvent& e) {
        if (e.type != EventType::Collision) return;
        const auto& collision = static_cast<const CollisionEvent&>(e); //cast base type to collision type

        onCollision(collision, "item", world);
        onCollision(collision, "wall", world);
        onCollision(collision, "projectile", world);
    });

    world.getEventManager().subscribe([this, &world](const BaseEvent& e) {
        if (e.type != EventType::PlayerAction) return;
        const auto& playerAction = static_cast<const PlayerActionEvent&>(e); //cast base type to collision type

        //TODO onPlayerAction
    });
}

void EventResponseSystem::onCollision(const CollisionEvent &e, const char *otherTag, World &world) {
    Entity* player = nullptr;
    Entity* other = nullptr;

    if (!getCollisionEntities(e, otherTag, player, other)) return;

    if (std::string(otherTag) == "item") {

        if (e.state != CollisionState::Enter) return;
        other->destroy();

        for (auto& entity : world.getEntities()) {
            if (!entity->hasComponent<SceneState>()) continue;

            //scene state
            auto &sceneState = entity->getComponent<SceneState>();
            sceneState.coinsCollected++;
            if (sceneState.coinsCollected > 1) {
                Game::onSceneChangeRequest("level2");
            }
        }
    } else if (std::string(otherTag) == "wall") {

        if (e.state != CollisionState::Stay) return;

        auto& t = player->getComponent<Transform>();
        t.position = t.oldPosition;

    } else if (std::string(otherTag) == "projectile") {

        if (e.state != CollisionState::Enter) return;

        //this logic is simple and direct
        //ideally we would only operate on data in an update function (hinting at transient entities)
        auto& health = player->getComponent<Health>();
        health.currentHealth--;

        Game::gameState.playerHealth = health.currentHealth;

        std::cout << health.currentHealth << std::endl;
        if (health.currentHealth <= 0) {
            player->destroy();
            Game::onSceneChangeRequest("gameover");
        }

    }
}


bool EventResponseSystem::getCollisionEntities(const CollisionEvent &e, const char *otherTag, Entity *&player, Entity *&other) {
    if (e.entityA == nullptr || e.entityB == nullptr) return false;
    if (!(e.entityA->hasComponent<Collider>() && e.entityB->hasComponent<Collider>())) return false;

    auto& colA = e.entityA->getComponent<Collider>();
    auto& colB = e.entityB->getComponent<Collider>();

    if (colA.tag == "player" && colB.tag == otherTag) {
        player = e.entityA;
        other = e.entityB;
    } else if (colA.tag == otherTag && colB.tag == "player") {
        player = e.entityB;
        other = e.entityA;
    }
    return player && other;
}

