//
// Created by WillO on 2025-10-30.
//
#include "EventResponseSystem.h"

#include "Game.h"
#include "World.h"
#include "Collision.h"

EventResponseSystem::EventResponseSystem(World &world) {
    //subscriptions
    world.getEventManager().subscribe([this, &world](const BaseEvent &e) {
        if (e.type != EventType::Collision) return;
        const auto &collision = static_cast<const CollisionEvent &>(e); //cast base type to collision type

        onCollision(collision, "player", "item", world);
        onCollision(collision, "player", "wall", world);
        onCollision(collision, "player", "projectile", world);
        onCollision(collision, "player", "player", world);
        onCollision(collision, "player", "inactiveBall", world);
        onCollision(collision, "projectile", "wall", world);
        onCollision(collision, "projectile", "projectile", world);
    });

    world.getEventManager().subscribe([this, &world](const BaseEvent &e) {
        if (e.type != EventType::PlayerAction) return;
        const auto &playerAction = static_cast<const PlayerActionEvent &>(e); //cast base type to collision type

        //TODO onPlayerAction
    });

    world.getEventManager().subscribe([this, &world](const BaseEvent &e) {
        if (e.type != EventType::MouseInteraction) return;
        const auto &mouseInteractionEvent = static_cast<const MouseInteractionEvent &>(e);
        onMouseInteraction(mouseInteractionEvent);
    });
}

void EventResponseSystem::onMouseInteraction(const MouseInteractionEvent &e) {
    if (!e.entity->hasComponent<Clickable>()) return;

    auto &clickable = e.entity->getComponent<Clickable>();

    switch (e.state) {
        case MouseInteractionState::Pressed:
            clickable.onPressed();
            break;
        case MouseInteractionState::Released:
            clickable.onReleased();
            break;
        case MouseInteractionState::Cancel:
            clickable.onCancel();
            break;
        default:
            break;
    }
}

void EventResponseSystem::onCollision(const CollisionEvent &e, const char *mainTag, const char *otherTag, World &world) {
    Entity *main = nullptr;
    Entity *other = nullptr;

    if (!getCollisionEntities(e, mainTag, otherTag, main, other)) return;

    if (std::string(mainTag) == "player")
        onPlayerCollision(e, main, other, otherTag, world);
    else if (std::string(mainTag) == "projectile")
        onProjectileCollision(e, main, other, otherTag, world);
}

void EventResponseSystem::onPlayerCollision(const CollisionEvent &e, Entity* player, Entity* other, const char *otherTag, World &world) {
    if (std::string(otherTag) == "item") {
        if (e.state != CollisionState::Enter) return;
        world.getAudioEventQueue().push(std::make_unique<AudioEvent>("collect"));
        other->destroy();

        for (auto &entity: world.getEntities()) {
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

        auto &t = player->getComponent<Transform>();
        t.position = t.oldPosition;
    } else if (std::string(otherTag) == "player") {
        if (e.state != CollisionState::Stay) return;

        auto &t1 = player->getComponent<Transform>();
        t1.position = t1.oldPosition;

        auto &t2 = other->getComponent<Transform>();
        t2.position = t2.oldPosition;
    } else if (std::string(otherTag) == "projectile") {
        if (e.state != CollisionState::Enter) return;

        //this logic is simple and direct
        //ideally we would only operate on data in an update function (hinting at transient entities)
        auto &health = player->getComponent<Health>();
        if (other->hasComponent<Ball>()) {
            auto& ball = other->getComponent<Ball>();
            if ((player->hasComponent<Player1Tag>() && ball.playerNum != 1) || (player->hasComponent<Player2Tag>() && ball.playerNum != 2)) {
                health.currentHealth--;
                world.getAudioEventQueue().push(std::make_unique<AudioEvent>("hurt"));
                world.getAudioEventQueue().push(std::make_unique<AudioEvent>("bounce"));
            } else {
                world.getAudioEventQueue().push(std::make_unique<AudioEvent>("bounce"));
            }
        } else {
            health.currentHealth--;
            world.getAudioEventQueue().push(std::make_unique<AudioEvent>("hurt"));
            world.getAudioEventQueue().push(std::make_unique<AudioEvent>("bounce"));
        }

        Game::gameState.playerHealth = health.currentHealth;

        std::cout << health.currentHealth << std::endl;
        if (health.currentHealth <= 0) {
            player->destroy();
            Game::onSceneChangeRequest("gameover");
        }

        // Prevent further movement
        auto &projectileTransform = other->getComponent<Transform>();
        auto &projectileCollider = other->getComponent<Collider>();
        auto &playerCollider = player->getComponent<Collider>();
        // Bounce off playerCollider
        Vector2D normal = Collision::getAABBCollisionNormal(projectileCollider, playerCollider);
        // Reflections
        auto &projectileVelocity = other->getComponent<Velocity>();
        Vector2D reflected = projectileVelocity.direction - 2 * Vector2D::dot(projectileVelocity.direction, normal) * normal;
        projectileVelocity.oldDirection = projectileVelocity.direction;
        projectileVelocity.direction = reflected.normalize();
        projectileTransform.position = projectileTransform.oldPosition;

    } else if (std::string(otherTag) == "inactiveBall") {
        if (e.state != CollisionState::Enter) return;

        auto& holder = player->getComponent<BallHolder>();
        if (holder.numBallsHeld < 2) {
            world.getAudioEventQueue().push(std::make_unique<AudioEvent>("pickup"));
            holder.numBallsHeld++;
            other->destroy();
        } else {
            cout << "Player was already holding too many balls" << endl;
        }

    }
}

void EventResponseSystem::onProjectileCollision(const CollisionEvent &e, Entity* projectile, Entity* other, const char *otherTag, World &world) {
    if (std::string(otherTag) == "wall") {
        if (e.state != CollisionState::Stay) return;
        world.getAudioEventQueue().push(std::make_unique<AudioEvent>("bounce"));
        // Prevent further movement
        auto &projectileTransform = projectile->getComponent<Transform>();
        auto &projectileCollider = projectile->getComponent<Collider>();
        auto &wallCollider = other->getComponent<Collider>();
        // Bounce off wallCollider
        Vector2D normal = Collision::getAABBCollisionNormal(projectileCollider, wallCollider);
        // Reflections
        auto &projectileVelocity = projectile->getComponent<Velocity>();
        Vector2D reflected = projectileVelocity.direction - 2 * Vector2D::dot(projectileVelocity.direction, normal) * normal;
        projectileVelocity.oldDirection = projectileVelocity.direction;
        projectileVelocity.direction = reflected.normalize();
        projectileTransform.position = projectileTransform.oldPosition;
    } else if (std::string(otherTag) == "projectile") {
        if (e.state != CollisionState::Stay) return;
        world.getAudioEventQueue().push(std::make_unique<AudioEvent>("bounce"));
        // Prevent further movement
        auto &projectileACollider = projectile->getComponent<Collider>();
        auto &projectileAVelocity = projectile->getComponent<Velocity>();
        auto &projectileATransform = projectile->getComponent<Transform>();
        auto &projectileBCollider = other->getComponent<Collider>();
        auto &projectileBVelocity = other->getComponent<Velocity>();
        auto &projectileBTransform = other->getComponent<Transform>();
        // Get respective normals
        Vector2D normalAB = Collision::getAABBCollisionNormal(projectileACollider, projectileBCollider);
        Vector2D normalBA = Collision::getAABBCollisionNormal(projectileBCollider, projectileACollider);
        // Reflect projectile A's velocity
        Vector2D projectileAReflected = projectileAVelocity.direction - 2 * Vector2D::dot(projectileAVelocity.direction, normalAB) * normalAB;
        projectileAVelocity.oldDirection = projectileAVelocity.direction;
        projectileAVelocity.direction = projectileAReflected.normalize();
        // Reflect projectile B's velocity
        Vector2D projectileBReflected = projectileBVelocity.direction - 2 * Vector2D::dot(projectileBVelocity.direction, normalBA) * normalBA;
        projectileBVelocity.oldDirection = projectileBVelocity.direction;
        projectileBVelocity.direction = projectileBReflected.normalize();
        // Keep the original positions/prevent entity penetration
        projectileATransform.position = projectileATransform.oldPosition;
        projectileBTransform.position = projectileBTransform.oldPosition;
    }
}

bool EventResponseSystem::getCollisionEntities(const CollisionEvent &e, const char *mainTag, const char *otherTag, Entity *&main, Entity *&other) {
    if (e.entityA == nullptr || e.entityB == nullptr) return false;
    if (!(e.entityA->hasComponent<Collider>() && e.entityB->hasComponent<Collider>())) return false;

    auto &colA = e.entityA->getComponent<Collider>();
    auto &colB = e.entityB->getComponent<Collider>();

    if (colA.tag == mainTag && colB.tag == otherTag) {
        main = e.entityA;
        other = e.entityB;
    } else if (colA.tag == otherTag && colB.tag == mainTag) {
        main = e.entityB;
        other = e.entityA;
    }
    return main && other;
}
