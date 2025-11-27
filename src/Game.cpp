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

        if (TTF_Init() != 1) {
            std::cout << "TTF_Init failed." << std::endl;
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

    // load fonts
    AssetManager::loadFont("fira", "../asset/fonts/fira.ttf", HUDSystem::getTitleSize());
    AssetManager::loadFont("arial", "../asset/fonts/arial.ttf", HUDSystem::getFontSize());

    //load audio
    audioManager.loadAudio("theme", "../asset/audio/glitter_blast.ogg");
    audioManager.loadAudio("collect", "../asset/audio/coin.ogg");
    audioManager.loadAudio("bounce", "../asset/audio/bounce.ogg");
    audioManager.loadAudio("hurt", "../asset/audio/hurt.ogg");
    audioManager.loadAudio("throw", "../asset/audio/throw.ogg");
    audioManager.loadAudio("pickup", "../asset/audio/pickup.ogg");

    //load assets
    AssetManager::loadAnimation("player", "../asset/animations/cat_animations.xml");
    AssetManager::loadAnimation("enemy", "../asset/animations/bird_animations.xml");

    //load scenes
    sceneManager.loadScene(SceneType::MainMenu,"mainmenu", nullptr, width, height);
    sceneManager.loadScene(SceneType::Gameplay,"level1", "../asset/map.tmx", width, height);
    sceneManager.loadScene(SceneType::Gameplay,"level2", "../asset/map2.tmx", width, height);
    sceneManager.loadScene(SceneType::GameOver, "gameover", nullptr, width, height);

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
        sceneManager.changeSceneDeferred(sceneName);
    };
}

void Game::handleEvents() {
    //SDL listens to the OS for input events and adds them to an event queue
    //SDL_Event event;

    //get all events in the event queue
    SDL_Event event;
    while (SDL_PollEvent(&event) && isRunning)
    {
        switch (event.type) {
            case SDL_EVENT_QUIT: //usually triggered when user closes window
                isRunning = false;
                break;
            default:
                // add event to vector
                events.push_back(event);
                break;
        }
    }
}

void Game::update() {
    sceneManager.update(deltaTime, events);

    // remember to clear out te old events
    events.clear();
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