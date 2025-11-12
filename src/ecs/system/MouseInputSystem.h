//
// Created by susan on 2025-11-05.
//

#ifndef INC_8552GAME_MOUSEINPUTSYSTEM_H
#define INC_8552GAME_MOUSEINPUTSYSTEM_H
#include "SDL3/SDL_events.h"

class World;

class MouseInputSystem {
    public:
    void update(World& world, const SDL_Event& event);
};

#endif //INC_8552GAME_MOUSEINPUTSYSTEM_H