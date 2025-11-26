//
// Created by WillO on 2025-09-09.
//

#include "Map.h"

#include <cmath>

#include "TextureManager.h"
#include <sstream>
#include <tinyxml2.h>

void Map::load(const char* path, SDL_Texture* ts) {
    tileset = ts;
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path);

    //parse width and height
    auto* mapNode = doc.FirstChildElement("map");
    mapProps.width = mapNode->IntAttribute("width");
    mapProps.height = mapNode->IntAttribute("height");
    // Custom properties
    auto* properties = mapNode->FirstChildElement("properties");
    if (properties) {
        // TODO: Don't assume that the scale is the only custom property
        auto* scaleProp = properties->FirstChildElement("property");
        mapProps.scale = scaleProp->FloatAttribute("value");
    }
    //parse terrain data
    auto* layer = mapNode->FirstChildElement("layer");
    auto* data = layer->FirstChildElement("data");
    std::string csv = data->GetText();
    std::stringstream ss(csv);
    tileData = std::vector(mapProps.height, std::vector<int>(mapProps.width));
    for (int i = 0; i < mapProps.height; i++) {
        for (int j = 0; j < mapProps.width; j++) {
            std::string val;
            //read chars until comma or EOS
            if (!std::getline(ss, val, ',')) break;
            tileData[i][j] = std::stoi(val); //string to int
        }
    }

    for (auto* objectGroup = layer->NextSiblingElement("objectgroup"); objectGroup != nullptr; objectGroup = objectGroup->NextSiblingElement("objectgroup")) {
        const char* groupName = objectGroup->Attribute("name");
        //wall collisions
        if (strcmp(groupName, "Collision Layer") == 0) {
            //create for loop with initialization, condition, and increment
            for (auto* obj = objectGroup->FirstChildElement("object"); obj != nullptr; obj = obj->NextSiblingElement("object")) {
                Collider coll;
                coll.rect.x = obj->FloatAttribute("x") * mapProps.scale;
                coll.rect.y = obj->FloatAttribute("y") * mapProps.scale;
                coll.rect.w = obj->FloatAttribute("width") * mapProps.scale;
                coll.rect.h = obj->FloatAttribute("height") * mapProps.scale;
                wallColliders.push_back(coll);
            };
        } else if (strcmp(groupName, "Divider Collider Layer") == 0) {
            //create for loop with initialization, condition, and increment
            auto* obj = objectGroup->FirstChildElement("object");
            if (obj != nullptr) {
                dividerCollider.rect.x = obj->FloatAttribute("x") * mapProps.scale;
                dividerCollider.rect.y = obj->FloatAttribute("y") * mapProps.scale;
                dividerCollider.rect.w = obj->FloatAttribute("width") * mapProps.scale;
                dividerCollider.rect.h = obj->FloatAttribute("height") * mapProps.scale;
            }
        } else if (strcmp(groupName, "Item Layer") == 0) {
            for (auto* item = objectGroup->FirstChildElement("object"); item != nullptr; item = item->NextSiblingElement("object")) {
                Vector2D itemPos;
                itemPos.x = item->FloatAttribute("x") * mapProps.scale;
                itemPos.y = item->FloatAttribute("y") * mapProps.scale;
                itemPositions.push_back(itemPos);
            };
        } else if (strcmp(groupName, "Hazard Layer") == 0) {
            for (auto* item = objectGroup->FirstChildElement("object"); item != nullptr; item = item->NextSiblingElement("object")) {
                SpawnPoint newPoint = parseSpawnPoint(item);
                // TODO: Spawner logic
            };
        } else if (strcmp(groupName, "Player1 Pickup Layer") == 0) {
            for (auto* item = objectGroup->FirstChildElement("object"); item != nullptr; item = item->NextSiblingElement("object")) {
                SpawnPoint newPoint = parseSpawnPoint(item);
                mapProps.player1PickupSpawns.push_back(newPoint);
            };
        } else if (strcmp(groupName, "Player2 Pickup Layer") == 0) {
            for (auto* item = objectGroup->FirstChildElement("object"); item != nullptr; item = item->NextSiblingElement("object")) {
                SpawnPoint newPoint = parseSpawnPoint(item);
                mapProps.player2PickupSpawns.push_back(newPoint);
            };
        } else if (strcmp(groupName, "Player Spawn Layer") == 0) {
            for (auto* item = objectGroup->FirstChildElement("object"); item != nullptr; item = item->NextSiblingElement("object")) {
                SpawnPoint newPoint = parseSpawnPoint(item);
                std::string name = item->Attribute("name");
                mapProps.playerSpawns.emplace(name, newPoint);
            };
        }
    }
}

void Map::draw(const Camera& cam) {
    SDL_FRect src{}, dest{};
    dest.w = dest.h = 16 * mapProps.scale;

    for (int row = 0; row < mapProps.height; row++) {
        for (int col = 0; col < mapProps.width; col++) {
            int type = tileData[row][col];

            float worldX =static_cast<float>(col) * dest.w;
            float worldY = static_cast<float>(row) * dest.h;

            //converting from world space to screen space
            dest.x = std::round(worldX - cam.view.x);
            dest.y = std::round(worldY - cam.view.y);

            src = indexToSpriteCoords(type, 16, 8);

            TextureManager::draw(tileset, &src, &dest);
        }
    }
}

SDL_FRect Map::indexToSpriteCoords(int index, int tileSize, int tilesetWidth) {
    SDL_FRect spriteCoords = SDL_FRect();
    int tilesetPixelWidth = (tilesetWidth * tileSize);
    // Get the starting x,y
    spriteCoords.x = (index - 1) * tileSize % tilesetPixelWidth;
    spriteCoords.y = static_cast<int>((index - 1) / tilesetWidth) * tileSize;
    spriteCoords.w = tileSize;
    spriteCoords.h = tileSize;

    return spriteCoords;
}

SpawnPoint Map::parseSpawnPoint(tinyxml2::XMLElement* item) const {
    SpawnPoint newPoint;
    newPoint.position.x = item->FloatAttribute("x") * mapProps.scale;
    newPoint.position.y = item->FloatAttribute("y") * mapProps.scale;
    auto* pointProperties = item->FirstChildElement("properties");
    if (pointProperties) {
        auto* propertyElement = pointProperties->FirstChildElement("property");
        float rotation = propertyElement->FloatAttribute("value");
        newPoint.direction = Vector2D(cos(rotation * M_PI / 180.0f), sin(rotation * M_PI / 180.0f));
    }
    return newPoint;
}
