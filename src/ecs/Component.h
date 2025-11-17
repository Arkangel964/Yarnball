//
// Created by WillO on 2025-09-16.
//

#ifndef TUTORIAL1_COMPONENT_H
#define TUTORIAL1_COMPONENT_H
#include <functional>
#include <SDL3/SDL_render.h>
#include "../utils/Vector2D.h"
#include <string>
#include <unordered_map>

#include "AnimationClip.h"
#include "Entity.h"

using namespace std;

struct Transform {
    Vector2D position{};
    float rotation{};
    float scale{};
    Vector2D oldPosition{};
};

//direction and speed
struct Velocity {
    Vector2D direction{};
    Vector2D oldDirection{};
    float speed{};
};

// UI rendering
enum class RenderLayer {
    World,
    UI,
};

struct Sprite {
    SDL_Texture* texture = nullptr;
    SDL_FRect src{};
    SDL_FRect dst{};
    RenderLayer renderLayer = RenderLayer::World;
    bool visible;
};

struct SoundEffect {
    const char* path = nullptr;
};

struct Music {
    const char* path = nullptr;
    float playTime = 0.0f;
    bool shouldLoop = true;
};

struct Collider {
    string tag;
    SDL_FRect rect{};
    bool enabled = true;
};

struct Animation {
    unordered_map<string, AnimationClip> clips{};
    string currentClip{};
    float time{}; //time is accumulated for the current frame
    int currentFrame{}; //index of the current frame in the clip
    float speed = 0.1f; //time per frame
};

struct Camera {
    SDL_FRect view;
    float worldWidth;
    float worldHeight;
};

struct TimedSpawner {
    float spawnInterval{};
    std::function<void()> spawnCallback{};
    float timer{};
};

//Game state for the current scene
struct SceneState {
    int coinsCollected = 0;
};

struct Health {
    int currentHealth{};
};

struct Clickable {
    std::function<void()> onPressed{};
    std::function<void()> onReleased{};
    std::function<void()> onCancel{};
    bool pressed = false;
};

struct Parent {
    Entity* parent = nullptr;
};

struct Children {
    std::vector<Entity*> children{};
};

struct PlayerTag{};
struct ProjectileTag{};


#endif //TUTORIAL1_COMPONENT_H