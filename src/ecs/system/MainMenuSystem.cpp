//
// Created by WillO on 2025-10-30.
//

#include "MainMenuSystem.h"

#include "Game.h"

void MainMenuSystem::update(const std::vector<SDL_Event>& events) {
    for (const auto& event: events) {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            switch (event.key.key) {
                case SDLK_SPACE:
                    Game::onSceneChangeRequest("level1");
                    break;
                case SDLK_ESCAPE:
                    Game::onSceneChangeRequest("exit");
                    break;
            }
        }
    }
}

