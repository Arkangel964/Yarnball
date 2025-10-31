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
    width = mapNode->IntAttribute("width");
    height = mapNode->IntAttribute("height");
    //parse terrain data
    auto* layer = mapNode->FirstChildElement("layer");
    auto* data = layer->FirstChildElement("data");
    std::string csv = data->GetText();
    std::stringstream ss(csv);
    tileData = std::vector(height, std::vector<int>(width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
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
                coll.rect.x = obj->FloatAttribute("x");
                coll.rect.y = obj->FloatAttribute("y");
                coll.rect.w = obj->FloatAttribute("width");
                coll.rect.h = obj->FloatAttribute("height");
                wallColliders.push_back(coll);
            };
        } else if (strcmp(groupName, "Item Layer") == 0) {
            for (auto* item = objectGroup->FirstChildElement("object"); item != nullptr; item = item->NextSiblingElement("object")) {
                Vector2D itemPos;
                itemPos.x = item->FloatAttribute("x");
                itemPos.y = item->FloatAttribute("y");
                itemPositions.push_back(itemPos);
            };
        }
    }
    /*
    //parse collider data
    auto* objectGroup = layer->NextSiblingElement("objectgroup");
    //create for loop with initialization, condition, and increment
    for (auto* obj = objectGroup->FirstChildElement("object"); obj != nullptr; obj = obj->NextSiblingElement("object")) {
        Collider coll;
        coll.rect.x = obj->FloatAttribute("x");
        coll.rect.y = obj->FloatAttribute("y");
        coll.rect.w = obj->FloatAttribute("width");
        coll.rect.h = obj->FloatAttribute("height");
        wallColliders.push_back(coll);
    };

    //parse item data
    auto* itemObjectGroup = objectGroup->NextSiblingElement("objectgroup");
    //create for loop with initialization, condition, and increment
    for (auto* item = itemObjectGroup->FirstChildElement("object"); item != nullptr; item = item->NextSiblingElement("object")) {
        Vector2D itemPos;
        itemPos.x = item->FloatAttribute("x");
        itemPos.y = item->FloatAttribute("y");
        itemPositions.push_back(itemPos);
    };
    */
}

void Map::draw(const Camera& cam) {
    SDL_FRect src{}, dest{};
    dest.w = dest.h = 32;

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int type = tileData[row][col];

            float worldX =static_cast<float>(col) * dest.w;
            float worldY = static_cast<float>(row) * dest.h;

            //converting from world space to screen space
            dest.x = std::round(worldX - cam.view.x);
            dest.y = std::round(worldY - cam.view.y);

            switch (type) {
                case 1:
                    //dirt
                    src.x = 0;
                    src.y = 0;
                    src.w = 32;
                    src.h = 32;
                    break;
                case 2:
                    //grass
                    src.x = 32;
                    src.y = 0;
                    src.w = 32;
                    src.h = 32;
                    break;
                case 4:
                    //water
                    src.x = 32;
                    src.y = 32;
                    src.w = 32;
                    src.h = 32;
                    break;
                default:
                    break;
            }

            TextureManager::draw(tileset, src, dest);
        }
    }
}


