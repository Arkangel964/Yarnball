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

    void update(const float dt, const SDL_Event& e) {
        world.update(dt, e, type);
    }

    void render() {
        world.render();
    }

    World world;

    const string getName() {return name;}

    private:
    std::string name;
    SceneType type;
    void createProjectile(Vector2D pos, Vector2D dir, float speed);
};

#endif //TUTORIAL1_SCENE_H