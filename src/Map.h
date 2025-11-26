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
    bool isUsed{false};
};

struct MapProperties {
    int width{}, height{};
    float scale{};
    std::unordered_map<std::string, SpawnPoint> playerSpawns;
    std::vector<SpawnPoint> player1PickupSpawns;
    std::vector<SpawnPoint> player2PickupSpawns;
    std::vector<SpawnPoint> hazardSpawns;
};

class Map {
public:
    Map() = default;
    ~Map() = default;

    void load(const char* path, SDL_Texture* ts);
    void draw(const Camera& cam);

    SDL_Texture* tileset = nullptr;
    MapProperties mapProps;
    std::vector<std::vector<std::vector<int>>> layeredTileData;
    std::vector<Collider> wallColliders;
    std::vector<Vector2D> itemPositions;
    Collider dividerCollider;

private:
    SDL_FRect indexToSpriteCoords(int index, int tileSize, int tilesetWidth);
    SpawnPoint parseSpawnPoint(tinyxml2::XMLElement *item) const;
};

#endif //TUTORIAL1_MAP_H