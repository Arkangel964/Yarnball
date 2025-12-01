#include "Scene.h"

#include "../manager/AssetManager.h"
#include "Game.h"

#define PLAYER_DST_SPRITE_SIZE 48.0f

#define PLAYER_ICON_SPRITE_SIZE 80.0f

#define PLAYER1_UI_OFFSET 10.0f
#define PLAYER2_UI_OFFSET 160.0f

#define POWERUP_CHANCE_PERCENT 5;

constexpr int PLAYER_LIVES = 9;

Scene::Scene(SceneType sceneType, const char *sceneName, const char *mapPath, const int windowWidth,
             const int windowHeight) : name(sceneName), type(sceneType) {
    if (sceneType == SceneType::MainMenu) {
        initStaticScene(windowWidth, windowHeight, "../asset/menu.png");
        return;
    }

    if (sceneType == SceneType::GameOver) {
        const char *path = Game::gameState.playerWon == 1 ?
        "../asset/gameover1.png" : "../asset/gameover2.png";
        initStaticScene(windowWidth, windowHeight, path);
        return;
    }

    initGameplay(mapPath, windowWidth, windowHeight);
}

void Scene::initStaticScene(int windowWidth, int windowHeight, const char *texturePath) {
    //camera
    auto &cam = world.createEntity();
    cam.addComponent<Camera>();

    //menu
    auto &menu(world.createEntity());
    auto menuTransform = menu.addComponent<Transform>(Vector2D(0, 0), 0.0f, 1.0f);

    SDL_Texture *menuTex = TextureManager::load(texturePath);
    SDL_FRect menuSrc{0, 0, static_cast<float>(windowWidth), static_cast<float>(windowHeight)};
    SDL_FRect menuDest{menuTransform.position.x, menuTransform.position.y, static_cast<float>(windowWidth),
        static_cast<float>(windowHeight)};
    menu.addComponent<Sprite>(menuTex, menuSrc, menuDest);

    menu.addComponent<Collider>("ui", menuDest);

    auto& clickable = menu.addComponent<Clickable>();
    clickable.onReleased = [] {
        Game::onSceneChangeRequest("level1");
    };
};

void Scene::initGameplay(const char *mapPath, int windowWidth, int windowHeight) {
    srand(time(nullptr));
    //load map
    world.getMap().load(mapPath, TextureManager::load("../asset/spritesheet.png"));

    for (auto &collider: world.getMap().wallColliders) {
        auto &e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto &coll = e.addComponent<Collider>("wall");
        coll.rect.x = collider.rect.x;
        coll.rect.y = collider.rect.y;
        coll.rect.w = collider.rect.w;
        coll.rect.h = collider.rect.h;
    }

    // Create middle divider
    auto &e = world.createEntity();
    Collider &dividerCollider = world.getMap().dividerCollider;
    e.addComponent<Transform>(Vector2D(dividerCollider.rect.x, dividerCollider.rect.y), 0.0f, 1.0f);
    auto &divider = e.addComponent<Collider>("divider");
    divider.rect.x = dividerCollider.rect.x;
    divider.rect.y = dividerCollider.rect.y;
    divider.rect.w = dividerCollider.rect.w;
    divider.rect.h = dividerCollider.rect.h;

    //Load the camera
    auto &cam = world.createEntity();
    SDL_FRect camView{};
    camView.x = 0;
    camView.y = 0;
    camView.w = static_cast<float>(windowWidth);
    camView.h = static_cast<float>(windowHeight);
    cam.addComponent<Camera>(camView, world.getMap().mapProps.width * 32, world.getMap().mapProps.height * 32);

    //reset balls available for spawning
    Game::gameState.availableBallsForSpawning = 6;
    Game::gameState.availableHazardsForSpawning = 4;
    Game::gameState.availablePowerupsForSpawning = 1;

    //Create the player1
    auto &player1 = createPlayerEntity("player1", "../asset/animations/cat_brown_anim.png");
    player1.addComponent<Keybinds>(SDLK_W, SDLK_S, SDLK_A, SDLK_D, SDLK_E);
    player1.addComponent<Player1Tag>();

    //Create the player2
    auto &player2 = createPlayerEntity("player2", "../asset/animations/cat_grey_anim.png");
    player2.addComponent<Keybinds>(SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_RSHIFT);
    player2.addComponent<Player2Tag>();

    // Create player 1's portrait
    createPlayerIcon(Vector2D(PLAYER1_UI_OFFSET, 15), "../asset/ui/player1icon.png");
    createPlayerLabels("Player 1 ", "player1", PLAYER1_UI_OFFSET+PLAYER_ICON_SPRITE_SIZE+15);

    // Create player 2's portrait
    createPlayerIcon(Vector2D(static_cast<float>(windowWidth)-PLAYER2_UI_OFFSET-PLAYER_ICON_SPRITE_SIZE, 15),
        "../asset/ui/player2icon.png");
    createPlayerLabels("Player 2 ", "player2", static_cast<float>(windowWidth)-PLAYER2_UI_OFFSET+15);

    //dodgeball spawner
    createPickupSpawner(3.0f);
    createHazardSpawner(2.5f);

    //add scene state
    auto &state(world.createEntity());
    state.addComponent<SceneState>();
};

Entity &Scene::createPlayerEntity(const char *spawnLocationName, const char *spritePath) {
    auto &player(world.createEntity());
    auto spawnPoint = world.getMap().mapProps.playerSpawns.find(spawnLocationName)->second;
    spawnPoint.position.x -= PLAYER_DST_SPRITE_SIZE / 2.0f;
    spawnPoint.position.y -= PLAYER_DST_SPRITE_SIZE / 2.0f;
    auto &playerTransform(player.addComponent<Transform>(spawnPoint.position, 0.0f, 1.0f));
    playerTransform.oldPosition = playerTransform.position;

    // Create player's stats
    player.addComponent<RigidBody>(240.0f, 240.0f);
    player.addComponent<Velocity>(Vector2D(0.0f, 0.0f), spawnPoint.direction, 0.0f);
    player.addComponent<Health>(PLAYER_LIVES);
    player.addComponent<BallHolder>();

    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture *tex = TextureManager::load(spritePath);
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst{playerTransform.position.x, playerTransform.position.y, PLAYER_DST_SPRITE_SIZE, PLAYER_DST_SPRITE_SIZE};
    player.addComponent<Sprite>(tex, playerSrc, playerDst);
    auto &playerCollider = player.addComponent<Collider>("player");
    playerCollider.rect.w = playerDst.w;
    playerCollider.rect.h = playerDst.h;

    return player;
}

Entity &Scene::createPlayerIcon(Vector2D iconPosition, const char *iconPath) {
    auto &playerIcon(world.createEntity());
    auto iconTransform = playerIcon.addComponent<Transform>(iconPosition, 0.0f, 1.0f);

    SDL_Texture *iconTex = TextureManager::load(iconPath);
    SDL_FRect iconSrc{0, 0, PLAYER_ICON_SPRITE_SIZE, PLAYER_ICON_SPRITE_SIZE};
    SDL_FRect iconDest{iconTransform.position.x, iconTransform.position.y, PLAYER_ICON_SPRITE_SIZE, PLAYER_ICON_SPRITE_SIZE};
    auto& iconSpite = playerIcon.addComponent<Sprite>(iconTex, iconSrc, iconDest);
    iconSpite.renderLayer = RenderLayer::UI;
    iconSpite.visible = true;

    return playerIcon;
}

Entity &Scene::createLabel(const std::string& text, const std::string &font, LabelType type, const std::string& cacheKey, Vector2D position) {
    auto& e(world.createEntity());
    Label label = {
        text,
        AssetManager::getFont(font),
        {255, 255, 255, 255},
        type,
        cacheKey
    };
    TextureManager::loadLabel(label);
    e.addComponent<Label>(label);
    e.addComponent<Transform>(position, 0.0f, 1.0f);
    return e;
}

void Scene::createPlayerLabels(std::string textPrefix, std::string cacheKeyPrefix, float xOffset) {
    std::string text = textPrefix+"title";
    std::string font = "fira";
    std::string cacheKey = cacheKeyPrefix+"Title";
    createLabel(text, font, LabelType::PlayerTitle, cacheKey, Vector2D(xOffset, 10.0f));

    text = textPrefix+"lives";
    font = "arial";
    cacheKey = cacheKeyPrefix+"Lives";
    createLabel(text, font, LabelType::Lives, cacheKey, Vector2D(xOffset, 45.0f));

    text = textPrefix+"yarnballs";
    cacheKey = cacheKeyPrefix+"Yarnballs";
    createLabel(text, font, LabelType::Yarnballs, cacheKey, Vector2D(xOffset, 75.0f));
}

void Scene::createPickupSpawner(float interval) {
    auto &ballSpawner(world.createEntity());
    ballSpawner.addComponent<TimedSpawner>(interval, [this] {
        if (Game::gameState.availableBallsForSpawning <= 0) return;
        auto &e(world.createDeferredEntity());
        // Get a valid spawn point from the side
        SpawnPoint *spawn = nullptr;
        auto &pickupSpawns = Game::gameState.spawnBallOnPlayer1Side
            ? world.getMap().mapProps.player1PickupSpawns : world.getMap().mapProps.player2PickupSpawns;
        auto &otherPickupSpawns = !Game::gameState.spawnBallOnPlayer1Side
            ? world.getMap().mapProps.player1PickupSpawns : world.getMap().mapProps.player2PickupSpawns;
        // First try to get a free spawn point on the correct side 2 * pickupSpawns.size()
        int numAttempts = 0;
        while (!spawn && numAttempts <= 2 * pickupSpawns.size()) {
            numAttempts++;
            int index = rand() % pickupSpawns.size();
            if (!Game::gameState.usedSpawnPoints.contains(&pickupSpawns[index])) {
                spawn = &pickupSpawns[index];
                Game::gameState.usedSpawnPoints.emplace(spawn, &e);
            }
        }
        // If we didn't get one, just loop through the list once to make sure we didn't get really unlucky in rng
        if (!spawn) {
            for (int index = 0; index < pickupSpawns.size(); index++) {
                if (!Game::gameState.usedSpawnPoints.contains(&pickupSpawns[index])) {
                    spawn = &pickupSpawns[index];
                    Game::gameState.usedSpawnPoints.emplace(spawn, &e);
                    break;
                }
            }
        }
        // If we STILL don't have one, get one from the opposite side
        if (!spawn) {
            for (int index = 0; index < otherPickupSpawns.size(); index++) {
                if (!Game::gameState.usedSpawnPoints.contains(&otherPickupSpawns[index])) {
                    spawn = &otherPickupSpawns[index];
                    Game::gameState.usedSpawnPoints.emplace(spawn, &e);
                    break;
                }
            }
        }

        int randomValForPowerup = rand() % 100;
        int chance = POWERUP_CHANCE_PERCENT;
        //guarantee that we spawn a ball if we can't spawn a powerup
        if (Game::gameState.availablePowerupsForSpawning <= 0) {
            randomValForPowerup = 100;
        }

        if (randomValForPowerup >= chance) {
            //Spawn a ball
            Game::gameState.availableBallsForSpawning -= 1;
            float spriteSize = 64.0f;
            float ballSize = 32.0f;
            Vector2D newPosition = spawn->position;
            newPosition.x -= ballSize / 2.0f;
            newPosition.y -= ballSize / 2.0f;
            e.addComponent<Transform>(newPosition, 0.0f, 1.0f);
            // Sprite setup
            SDL_Texture *tex = TextureManager::load("../asset/yarnball_blue_stationary.png");
            SDL_FRect spriteSrc(0, 0, spriteSize, spriteSize);
            SDL_FRect dest(newPosition.x, newPosition.y, ballSize, ballSize);
            e.addComponent<Sprite>(tex, spriteSrc, dest);
            // Collider setup
            auto &c = e.addComponent<Collider>("inactiveBall");
            c.rect.w = dest.w;
            c.rect.h = dest.h;
        } else {
            //Spawn a powerup instead
            if (Game::gameState.availablePowerupsForSpawning <= 0) return;
            Game::gameState.availablePowerupsForSpawning -= 1;
            float spriteSize = 31.0f;
            float starSize = 31.0f;
            Vector2D newPosition = spawn->position;
            newPosition.x -= starSize / 2.0f;
            newPosition.y -= starSize / 2.0f;
            e.addComponent<Transform>(newPosition, 0.0f, 1.0f);
            // Sprite setup
            SDL_Texture *tex = TextureManager::load("../asset/star.png");
            SDL_FRect spriteSrc(0, 0, spriteSize, spriteSize);
            SDL_FRect dest(newPosition.x, newPosition.y, starSize, starSize);
            e.addComponent<Sprite>(tex, spriteSrc, dest);
            // Collider setup
            auto &c = e.addComponent<Collider>("powerup");
            c.rect.w = dest.w;
            c.rect.h = dest.h;
        }

        // Flip which side should go next
        Game::gameState.spawnBallOnPlayer1Side = !Game::gameState.spawnBallOnPlayer1Side;
    });
}

void Scene::createHazardSpawner(float interval) {
    auto &spawnPoints = world.getMap().mapProps.hazardSpawns;
    if (spawnPoints.empty()) return;

    auto &spawner(world.createEntity());
    spawner.addComponent<TimedSpawner>(interval, [this, spawnPoints] {
        if (Game::gameState.availableHazardsForSpawning > 0) {
            Game::gameState.availableHazardsForSpawning -= 1;
            float spriteSize = 50.0f;
            float ballSize = 25.0f;
            // create our projectile (ball)
            auto &e(world.createDeferredEntity());
            float index = rand() % spawnPoints.size();
            SpawnPoint spawn = spawnPoints[index];
            spawn.position.x -= ballSize / 2.0f;
            spawn.position.y -= ballSize / 2.0f;
            e.addComponent<Transform>(spawn.position, 0.0f, 1.0f);

            SDL_Texture *tex = TextureManager::load("../asset/yarnball_pink_moving.png");
            SDL_FRect spriteSrc(0, 0, spriteSize, spriteSize);
            SDL_FRect dest(spawn.position.x, spawn.position.y, ballSize, ballSize);
            e.addComponent<Sprite>(tex, spriteSrc, dest);

            auto &c = e.addComponent<Collider>("projectile");
            c.rect.w = dest.w;
            c.rect.h = dest.h;

            e.addComponent<RigidBody>(500.0f, 3.0f);
            e.addComponent<ProjectileTag>();
            e.addComponent<DestroyOnStop>([] {
                Game::gameState.availableHazardsForSpawning += 1;
            });
            PhysicsSystem::addImpulse(e, spawn.direction, 500.0f);
        }
    });
}

