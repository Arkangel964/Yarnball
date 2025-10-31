//
// Created by WillO on 2025-09-09.
//

#ifndef TUTORIAL1_MAP_H
#define TUTORIAL1_MAP_H
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>
#include <Component.h>
#include <vector>

class Map {
    public:
        Map() = default;
        ~Map() = default;

        void load(const char* path, SDL_Texture* ts);
        void draw(const Camera& cam);

        SDL_Texture* tileset = nullptr;
        int width{}, height{};
        std::vector<std::vector<int>> tileData;
        std::vector<Collider> wallColliders;
        std::vector<Vector2D> itemPositions;

};

#endif //TUTORIAL1_MAP_H