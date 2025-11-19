//
// Created by WillO on 2025-09-09.
//

#ifndef TUTORIAL1_TEXTUREMANAGER_H
#define TUTORIAL1_TEXTUREMANAGER_H


#include <string>
#include <unordered_map>

#include "Component.h"
#include "SDL3_image/SDL_image.h"

class TextureManager {
    static std::unordered_map<std::string, SDL_Texture*> textures;
public:
    static SDL_Texture* load(const char* path);

    static void loadLabel(Label& label);
    static void updateLabel(Label& label);

    //floating point rects = FRect
    static void draw(SDL_Texture* texture, const SDL_FRect* src, const SDL_FRect* dst);

    static void clean();
};

#endif //TUTORIAL1_TEXTUREMANAGER_H