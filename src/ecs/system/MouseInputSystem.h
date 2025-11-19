//
// Created by susan on 2025-11-05.
//

#ifndef INC_8552GAME_MOUSEINPUTSYSTEM_H
#define INC_8552GAME_MOUSEINPUTSYSTEM_H
#include <vector>

#include "SDL3/SDL_events.h"

class World;

class MouseInputSystem {
    public:
    void update(World& world, const std::vector<SDL_Event>& events);
};

#endif //INC_8552GAME_MOUSEINPUTSYSTEM_H