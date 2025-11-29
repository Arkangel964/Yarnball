//
// Created by WillO on 2025-10-21.
//

#ifndef TUTORIAL1_SCENE_H
#define TUTORIAL1_SCENE_H
#include "Vector2D.h"
#include "World.h"
#include "SceneType.h"

class Scene {
    public:
    Scene(SceneType sceneType, const char* sceneName, const char* mapPath, int windowWidth, int windowHeight);

    void update(const float dt, const std::vector<SDL_Event>& events) {
        world.update(dt, events, type);
    }

    void render() {
        world.render();
    }

    World world;

    const string getName() {return name;}

    private:
    std::string name;
    SceneType type;

    void initMainMenu(int windowWidth, int windowHeight);
    void initGameplay(const char* mapPath, int windowWidth, int windowHeight);
    void initGameOver(int windowWidth, int windowHeight);

    Entity& createSettingsOverlay(int windowWidth, int windowHeight);
    Entity& createCogButton(int windowWidth, int windowHeight, Entity& overlay);
    void createSettingUIComponents(Entity& overlay);
    void toggleSettingsOverlayVisibility(Entity& overlay);

    Entity &createPlayerEntity(const char *spawnLocationName, const char *spritePath);
    Entity &createPlayerIcon(Vector2D iconPosition, const char *iconPath);

    Entity &createLabel(const std::string &text, const std::string &font, LabelType type, const std::string &cacheKey, Vector2D position);
    void createPlayerLabels(std::string textPrefix, std::string cacheKeyPrefix, float xOffset);

    void createPickupSpawner(float interval);
    void createHazardSpawner(float interval);
};

#endif //TUTORIAL1_SCENE_H