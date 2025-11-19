#include "TextureManager.h"
#include "Game.h"

//doesn't create any extra memory, just grabs the reference of this object
extern Game* game;
std::unordered_map<std::string, SDL_Texture*> TextureManager::textures;

//load a texture with a path
SDL_Texture* TextureManager::load(const char* path) {\

    //check the cache first
    auto it = textures.find(path);
    if (it != textures.end()) {
        return it->second;
    }

    //A surface represents and image in RAM (cpu)
    //do we want to do anything with the image before storing it in gpu?
    SDL_Surface* tempSurface = IMG_Load(path);
    if (!tempSurface) {
        printf("Failed to load image: %s\n", path);
    }

    //texture represents an image in VRAM (gpu)
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, tempSurface);

    SDL_DestroySurface(tempSurface);

    if (!texture) {
        printf("Failed to create texture: %s\n", path);
        return nullptr;
    }

    //cache the texture
    textures[path] = texture;

    return texture;
}

void TextureManager::draw(SDL_Texture* texture, const SDL_FRect* src, const SDL_FRect* dst) {
    //draw the texture to the screen from vram
    SDL_RenderTexture(game->renderer, texture, src, dst);

}

void TextureManager::clean() {
    for (auto& tex : textures) {
        SDL_DestroyTexture(tex.second);
        tex.second = nullptr;
    }
    textures.clear();
}