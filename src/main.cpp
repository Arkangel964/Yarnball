#include <iostream>
#include "Game.h"

//global variable
Game *game = nullptr;

int main() {
    const int FPS = 60; //60 is the closest refresh rate of most monitors
    const int desiredFrameTime = 1000/FPS; //16ms per frame

    Uint64 ticks{};
    Uint64 currentTicks{};
    int actualFrameTime = 0;

    game = new Game();
    game->init("8552 Tutorial Engine", 800, 600, false);

    // !!! -- GAME LOOP -- !!!
    while (game->running()) {
        currentTicks = SDL_GetTicks(); //time in ms since SDL init
        game->deltaTime = static_cast<float>(currentTicks - ticks) / 1000.0f;
        // printf("%f", game->deltaTime);
        ticks = currentTicks;
        game->handleEvents();
        game->update();
        game->render();
        actualFrameTime = static_cast<int>(SDL_GetTicks() - ticks); //elapsed time in ms it took the current frame

        //cap the framerate so that we aren't running over 60fps
        //if the frame took less time than what we intend, we artificially add delay before starting next frame
        if (actualFrameTime < desiredFrameTime) {
            SDL_Delay(desiredFrameTime - actualFrameTime);
        }
    }

    delete game;

    return 0;
}