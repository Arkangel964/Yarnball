//
// Created by WillO on 2025-10-30.
//

#ifndef TUTORIAL1_MAINMENUSYSTEM_H
#define TUTORIAL1_MAINMENUSYSTEM_H
#include <vector>
#include <SDL3/SDL_events.h>

#include "scene/SceneType.h"

class MainMenuSystem {
    public:
    void update(const std::vector<SDL_Event>& events, SceneType sceneType);

};

#endif //TUTORIAL1_MAINMENUSYSTEM_H