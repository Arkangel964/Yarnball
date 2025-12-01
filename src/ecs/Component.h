//
// Created by WillO on 2025-09-16.
//

#ifndef TUTORIAL1_COMPONENT_H
#define TUTORIAL1_COMPONENT_H
#include <functional>
#include <SDL3/SDL_render.h>
#include "SDL3_ttf/SDL_ttf.h"
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

struct Impulse {
    Vector2D direction{};
    float force{};
};

//direction and speed
struct Velocity {
    Vector2D direction{};
    Vector2D oldDirection{};
    float speed{};
};

struct RigidBody {
    float maxSpeed{};   // Cap speed to a specific value
    float friction{};   // Apply decay per frame to slow entities down
};

struct DestroyOnStop{
    std::function<void()> onDestroy{};
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

struct Keybinds {
    SDL_Keycode up{};
    SDL_Keycode down{};
    SDL_Keycode left{};
    SDL_Keycode right{};
    SDL_Keycode ball{};
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

struct Invincibility {
    float duration{};
    //the interval at which the sprite should flash while invincible
    float flashInterval{};
    float timeElapsedCurrent = 0;
    float flashTimeElapsedCurrent = 0;
    bool flash = false;
};

struct SpeedBoost {
    float speedMultiplier{};
    float duration{};
    float timeElapsedCurrent = 0;
};

struct BallHolder {
    int numBallsHeld = 0;
    bool shouldThrowBall = false;
    float cooldown = 0.2f;
};

struct Ball {
    int playerNum = 0;
};

enum class LabelType {
    Lives,
    Yarnballs,
    PlayerTitle
};

struct Label {
    std::string text{};
    TTF_Font* font = nullptr;
    SDL_Color color{};
    LabelType type = LabelType::PlayerTitle;
    std::string textureCacheKey{};
    SDL_Texture* texture = nullptr;
    SDL_FRect dst{};
    bool visible = true;
    bool dirty = false;
};

struct Player1Tag{};
struct Player2Tag{};
struct ProjectileTag{};

#endif //TUTORIAL1_COMPONENT_H