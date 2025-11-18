//
// Created by WillO on 2025-10-30.
//

#ifndef TUTORIAL1_EVENTRESPONSESYSTEM_H
#define TUTORIAL1_EVENTRESPONSESYSTEM_H
#include <functional>
#include <vector>

#include "event/BaseEvent.h"

class Entity;
class World;

class EventResponseSystem {
public:
    EventResponseSystem(World& world);
private:
    //collisions
    void onCollision(const CollisionEvent &e, const char *mainTag, const char *otherTag, World &world);
    bool getCollisionEntities(const CollisionEvent &e, const char *mainTag, const char *otherTag, Entity *&main, Entity *&other);
    void onPlayerCollision(const CollisionEvent &e, Entity *player, Entity *other, const char *otherTag, World &world);
    void onProjectileCollision(const CollisionEvent &e, Entity *projectile, Entity *other, const char *otherTag, World &world);

    //player action
    void onPlayerAction(const PlayerActionEvent& e,
        const std::function<void(Entity* player, PlayerAction action)>& callback);

    void onMouseInteraction(const MouseInteractionEvent& e);
};

#endif //TUTORIAL1_EVENTRESPONSESYSTEM_H