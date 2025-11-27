//
// Created by WillO on 2025-10-30.
//

#include "MainMenuSystem.h"

#include "Game.h"

void MainMenuSystem::update(const std::vector<SDL_Event>& events, SceneType sceneType) {
    for (const auto& event: events) {
        if (event.type == SDL_EVENT_KEY_DOWN) {
            switch (event.key.key) {
                case SDLK_SPACE:
                    Game::onSceneChangeRequest(sceneType == SceneType::MainMenu ? "level1" : "mainmenu");
                    break;
            }
        }
    }
}

