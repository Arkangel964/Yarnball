#include "GameObject.h"

#include "TextureManager.h"

GameObject::GameObject(const char *texpath, int x, int y) :
xPos(x), yPos(y), texture(TextureManager::load(texpath))
{}

GameObject::~GameObject() {
    SDL_DestroyTexture(texture);
}

void GameObject::update(float deltaTime) {
    //move gameobject one pixel on the x and y each frame
    xPos += speed * deltaTime;
    yPos += speed * deltaTime;

    //where do we want to crop the texture from?
    srcRect.x = srcRect.y = 0;
    //how much to crop?
    srcRect.w = srcRect.h = 32;

    //where to draw it?
    destRect.x = xPos; destRect.y = yPos;
    //how big?
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;
}

void GameObject::draw() {
    TextureManager::draw(texture, &srcRect, &destRect);
}
