//
// Created by WillO on 2025-10-30.
//

#ifndef TUTORIAL1_BASEEVENT_H
#define TUTORIAL1_BASEEVENT_H

class Entity;

enum class EventType {
  Collision,
  PlayerAction,
};

struct BaseEvent {
  EventType type{};
  virtual ~BaseEvent() = default; //in c++ always make the destructor virtual from a class you are deriving from
};

enum class CollisionState { Enter, Stay, Exit };

struct CollisionEvent : BaseEvent {
  Entity* entityA = nullptr;
  Entity* entityB = nullptr;
  CollisionState state{};

  CollisionEvent(Entity* entityA, Entity* entityB, CollisionState state) : entityA(entityA), entityB(entityB), state(state) {
    type = EventType::Collision;
  }
};

enum class PlayerAction {Attack, Interact, useItem, Jump };

struct PlayerActionEvent : BaseEvent {
  Entity* player = nullptr;
  PlayerAction action{};
  PlayerActionEvent(Entity* player, PlayerAction action) : player(player), action(action) {
    type = EventType::PlayerAction;
  }
};

#endif //TUTORIAL1_BASEEVENT_H