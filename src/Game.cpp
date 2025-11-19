//
// Created by WillO on 2025-09-02.
//

#include "Game.h"
#include "Map.h"

#include <ctime>
#include <iostream>
#include <ostream>

#include "manager/AssetManager.h"

//#include "GameObject.h"

GameState Game::gameState{};
std::function<void(string)> Game::onSceneChangeRequest;

Game::Game() {

}

Game::~Game() {
    destroy();
}

void Game::init(const char* title, int width, int height, bool fullscreen) {
    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    //initialize SDL library
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) == 1) {
        std::cout << "Subsystem initialized..." << std::endl;
        window = SDL_CreateWindow(title, width, height, flags);
        if (window) {
            std::cout << "Window created..." << std::endl;
        } else {
            std::cout << "Window could not be created." << std::endl;
        }

        //windows will be direct3d (directx)
        //mac is likely Metal or OpenGL
        renderer = SDL_CreateRenderer(window, nullptr);

        if (renderer) {
            std::cout << "Renderer created..." << std::endl;
        } else {
            std::cout << "Renderer could not be created." << std::endl;
            return;
        }
        isRunning = true;
        //color initialization and randomization seeding
        srand(time(NULL));
        r = rand() % 255;
        g = rand() % 255;
        b = rand() % 255;
    } else {
        std::cout << "Subsystem could not be initialized." << std::endl;
        isRunning = false;
    }

    //load audio
    audioManager.loadAudio("theme", "../asset/audio/switch_with_me.ogg");
    audioManager.loadAudio("collect", "../asset/audio/coin.ogg");


    //load assets
    AssetManager::loadAnimation("player", "../asset/animations/cat_animations.xml");
    AssetManager::loadAnimation("enemy", "../asset/animations/bird_animations.xml");

    //init game data/state
    gameState.playerHealth = 5;

    //load scenes
    sceneManager.loadScene(SceneType::MainMenu,"mainmenu", nullptr, width, height);
    sceneManager.loadScene(SceneType::Gameplay,"level1", "../asset/map.tmx", width, height);
    sceneManager.loadScene(SceneType::Gameplay,"level2", "../asset/map2.tmx", width, height);

    //start music
    audioManager.playMusic("theme");

    //start level 1
    sceneManager.changeSceneDeferred("mainmenu");



    //resolve scene callback
    onSceneChangeRequest = [&](string sceneName) {
        if (sceneManager.currentScene->getName() == "level2" && sceneName == "level2"){
            std::cout << "You win!" << std::endl;
            isRunning = false;
            return;
        }

        if (sceneName == "gameover") {
            std::cout << "Game Over :(" << std::endl;
            isRunning = false;
            return;
        }

        sceneManager.changeSceneDeferred(sceneName);
    };
}

void Game::handleEvents() {
    //SDL listens to the OS for input events and adds them to an event queue
    //SDL_Event event;

    //check for next event, if an event is available it pops from queue and store in event
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_EVENT_QUIT: //usually triggered when user closes window
            isRunning = false;
            break;
        default:
            break;
    }
}

void Game::update() {
    sceneManager.update(deltaTime, event);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    //every frame the renderer gets cleared with the draw color set above ^^
    SDL_RenderClear(renderer);

    //vvv -- all our draw events would go here, order is relevant -- vvv
    sceneManager.render();

    //display all the render changes that happened above
    //draws it in memory first to a back buffer, then this swaps the back buffer to the screen
    SDL_RenderPresent(renderer);
}

void Game::destroy() {
    TextureManager::clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game has been cleaned up." << std::endl;
}