#include "TextureManager.h"
#include "Game.h"

//doesn't create any extra memory, just grabs the reference of this object
extern Game* game;
std::unordered_map<std::string, SDL_Texture*> TextureManager::textures;

//load a texture with a path
SDL_Texture* TextureManager::load(const char* path) {

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

void TextureManager::loadLabel(Label &label) {
    auto it = textures.find(label.textureCacheKey);
    if (it != textures.end()) {
        label.texture = it->second;
        return;
    }

    // update label
    updateLabel(label);

}

void TextureManager::updateLabel(Label &label) {
    if (!label.dirty) return;
    if (label.texture) {
        SDL_DestroyTexture(label.texture);
        label.texture = nullptr;
    }

    SDL_Surface* tempSurface = TTF_RenderText_Blended(
        label.font, label.text.c_str(), label.text.size(), label.color
        );

    if (!tempSurface) {
        std::cout << "Failed to load surface: " << label.textureCacheKey << std::endl;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, tempSurface);
    SDL_DestroySurface(tempSurface);

    if (!texture) {
        std::cout << "Failed to create texture: " << label.textureCacheKey << std::endl;
    }
    float w, h;
    SDL_GetTextureSize(texture, &w, &h);
    label.dst.w = w;
    label.dst.h = h;

    // cache
    label.texture = texture;
    textures[label.textureCacheKey] = texture;

    label.dirty = false;
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