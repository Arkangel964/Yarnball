//
// Created by WillO on 2025-09-09.
//

#ifndef TUTORIAL1_GAMEOBJECT_H
#define TUTORIAL1_GAMEOBJECT_H

#include "Game.h"

class GameObject {
public:
    GameObject(const char* texpath, int x, int y);
    ~GameObject();

    void update(float deltaTime);
    void draw();

private:
    int xPos{}, yPos{};
    float speed = 100.0f;
    SDL_Texture* texture = nullptr;
    SDL_FRect srcRect{}, destRect{};
};

#endif //TUTORIAL1_GAMEOBJECT_H