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
#include "tinyxml2.h"

struct SpawnPoint {
    Vector2D position;
    Vector2D direction;
};

struct MapProperties {
    int width{}, height{};
    float scale{};
    std::unordered_map<std::string, SpawnPoint> playerSpawns;
//    std::vector<SpawnPoint> ballSpawns;
//    std::vector<SpawnPoint> hazardSpawns;
};

class Map {
public:
    Map() = default;
    ~Map() = default;

    void load(const char* path, SDL_Texture* ts);
    void draw(const Camera& cam);

    SDL_Texture* tileset = nullptr;
    MapProperties mapProps;
    std::vector<std::vector<int>> tileData;
    std::vector<Collider> wallColliders;
    std::vector<Vector2D> itemPositions;

private:
    SDL_FRect indexToSpriteCoords(int index, int tileSize, int tilesetWidth);
    SpawnPoint parseSpawnPoint(tinyxml2::XMLElement *item) const;
};

#endif //TUTORIAL1_MAP_H