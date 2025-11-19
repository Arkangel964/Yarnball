//
// Created by WillO on 2025-10-21.
//

#ifndef TUTORIAL1_SCENEMANAGER_H
#define TUTORIAL1_SCENEMANAGER_H
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "../scene/Scene.h"

using namespace std;

struct SceneParams {
    SceneType sceneType;
    const char* name;
    const char* path;
    int windowWidth;
    int windowHeight;
};

class SceneManager {
    unordered_map<string, SceneParams> sceneParams;
    string pendingScene;


    void changeScene(const string& name) {
        auto it = sceneParams.find(name);
        if (it != sceneParams.end()) {
            const auto& params = it->second;
            //create scene object
            currentScene = make_unique<Scene>(params.sceneType, params.name, params.path, params.windowWidth, params.windowHeight);
        } else {
            cerr << "Scene " << name << " not found." << endl;
        }
    }

public:
    unique_ptr<Scene> currentScene;
    void loadScene(SceneType sceneType, const char* sceneName, const char* mapPath, int windowWidth, int windowHeight) {
        sceneParams[sceneName] = {sceneType, sceneName, mapPath, windowWidth, windowHeight};
    }

    void changeSceneDeferred(const string& name) {
        pendingScene = name;
    }

    void update(const float dt, const std::vector<SDL_Event>& events) {
        if (currentScene) currentScene->update(dt, events);
        if (!pendingScene.empty()) {
            changeScene(pendingScene);
            pendingScene.clear();
        }
    }

    void render() {
        if (currentScene) currentScene->render();
    }

};

#endif //TUTORIAL1_SCENEMANAGER_H