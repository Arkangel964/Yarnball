//
// Created by WillO on 2025-09-02.
//

#pragma once
#include <random>
#include <SDL3/SDL.h>

#include "World.h"
#include "manager/AudioManager.h"
#include "manager/SceneManager.h"

using namespace std;

struct GameState {
    int playerHealth;
    int availableBallsForSpawning;
    int playerWon = 1;
    int remainingLives = 0;
    int numBallsThrown = 0;
};

class Game {
public:
    Game();
    ~Game();

    //these are the types that SDL uses
    void init(const char* title, int width, int height, bool fullscreen);

    //game loop stuff (handleEvents, update, render)
    void handleEvents(); //checks for input and system events
    void update(); //game logic and state changes
    void render(); //draw game changes to screen

    //resource cleanup
    void destroy();

    bool running() {
        return isRunning;
    }

    SDL_Renderer *renderer = nullptr;

    SceneManager sceneManager;
    AudioManager audioManager;
    static function<void(string)> onSceneChangeRequest;
    static GameState gameState;

    float deltaTime = 0.0f;

private:
    int frameCount = 0;
    bool isRunning = false;

    SDL_Window* window = nullptr;
    //SDL_Event event;
    std::vector<SDL_Event> events;


    //sdl stores color channels as 8-bit unsigned
    //0-255
    Uint32 r,g,b,a = 255;
};
