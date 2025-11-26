#include "Scene.h"

#include "../manager/AssetManager.h"
#include "Game.h"

#define PLAYER2_UI_Offset 160.0f

Scene::Scene(SceneType sceneType, const char *sceneName, const char *mapPath, const int windowWidth,
             const int windowHeight) : name(sceneName), type(sceneType) {
    if (sceneType == SceneType::MainMenu) {
        initMainMenu(windowWidth, windowHeight);
        return;
    }

    initGameplay(mapPath, windowWidth, windowHeight);
}

void Scene::initMainMenu(int windowWidth, int windowHeight) {
    //camera
    auto &cam = world.createEntity();
    cam.addComponent<Camera>();

    //menu
    auto &menu(world.createEntity());
    auto menuTransform = menu.addComponent<Transform>(Vector2D(0, 0), 0.0f, 1.0f);

    SDL_Texture *menuTex = TextureManager::load("../asset/menu.png");
    SDL_FRect menuSrc{0, 0, 800, 600};
    SDL_FRect menuDest{menuTransform.position.x, menuTransform.position.y, (float) windowWidth, (float) windowHeight};
    menu.addComponent<Sprite>(menuTex, menuSrc, menuDest);

    auto &settingsOverlay = createSettingsOverlay(windowWidth, windowHeight);
    createCogButton(windowWidth, windowHeight, settingsOverlay);
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

//        SDL_Texture *tex = TextureManager::load("../asset/spritesheet.png");
//        SDL_FRect colSrc{0, 32, 32, 32};
//        SDL_FRect colDst{coll.rect.x, coll.rect.y, coll.rect.w, coll.rect.h};
//        e.addComponent<Sprite>(tex, colSrc, colDst);
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
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, world.getMap().mapProps.width * 32, world.getMap().mapProps.height * 32);

    //reset balls available for spawning
    Game::gameState.availableBallsForSpawning = 6;
    Game::gameState.availableHazardsForSpawning = 4;

    //Create the player1
    auto &player1 = createPlayerEntity("player1", "../asset/animations/cat_brown_anim.png");
    player1.addComponent<Keybinds>(SDLK_W, SDLK_S, SDLK_A, SDLK_D, SDLK_E);
    player1.addComponent<Player1Tag>();

    //Create the player2
    auto &player2 = createPlayerEntity("player2", "../asset/animations/cat_grey_anim.png");
    player2.addComponent<Keybinds>(SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_RSHIFT);
    player2.addComponent<Player2Tag>();

    // Create player 1's portrait
    createPlayerIcon(Vector2D(10, 15), "../asset/ui/player1icon.png");
    createPlayerTitleLabel(player1, windowWidth, windowHeight);
    createPlayerLivesLabel(player1, windowWidth, windowHeight);
    createPlayerYarnballsLabel(player1, windowWidth, windowHeight);

    // Create player 2's portrait
    createPlayerIcon(Vector2D(windowWidth-PLAYER2_UI_Offset-80, 15), "../asset/ui/player2icon.png");
    createPlayerTitleLabel(player2, windowWidth, windowHeight);
    createPlayerLivesLabel(player2, windowWidth, windowHeight);
    createPlayerYarnballsLabel(player2, windowWidth, windowHeight);

    //dodgeball spawner
    createPickupSpawner(5.0f);
    createHazardSpawner(1.f);

    //add scene state
    auto &state(world.createEntity());
    state.addComponent<SceneState>();
};

Entity &Scene::createSettingsOverlay(int windowWidth, int windowHeight) {
    auto &overlay(world.createEntity());
    SDL_Texture *overlayTex = TextureManager::load("../asset/ui/settings.jpg");
    SDL_FRect overlaySrc{0, 0, (float) windowWidth * 0.85f, (float) windowHeight * 0.85f};
    SDL_FRect overlayDest{
        (float) windowWidth / 2 - overlaySrc.w / 2, (float) windowHeight / 2 - overlaySrc.h / 2, overlaySrc.w,
        overlaySrc.h
    };
    overlay.addComponent<Transform>(Vector2D(overlayDest.x, overlayDest.y), 0.0f, 1.0f);
    overlay.addComponent<Sprite>(overlayTex, overlaySrc, overlayDest, RenderLayer::UI, false);
    createSettingUIComponents(overlay);
    return overlay;
};

Entity &Scene::createCogButton(int windowWidth, int windowHeight, Entity &overlay) {
    auto &cog(world.createEntity());
    auto &cogTransform = cog.addComponent<Transform>(Vector2D((float) windowWidth - 50, (float) windowHeight - 50),
                                                     0.0f, 1.0f);
    SDL_Texture *text = TextureManager::load("../asset/ui/cog.png");
    SDL_FRect cogSrc{0, 0, 32, 32};
    SDL_FRect cogDest{cogTransform.position.x, cogTransform.position.y, cogSrc.w, cogSrc.h};
    cog.addComponent<Sprite>(text, cogSrc, cogDest, RenderLayer::UI, true);
    cog.addComponent<Collider>("ui", cogDest);

    auto& clickable = cog.addComponent<Clickable>();
    clickable.onPressed = [&cogTransform] {
        cogTransform.scale = 0.5f;
    };
    clickable.onReleased = [this, &cogTransform, &overlay] {
        cogTransform.scale = 1.0f;
        toggleSettingsOverlayVisibility(overlay);
    };
    clickable.onCancel = [&cogTransform] {
        cogTransform.scale = 1.0f;
    };
    return cog;

};

void Scene::createSettingUIComponents(Entity &overlay) {
    if (!overlay.hasComponent<Children>()) {
        overlay.addComponent<Children>();
    }

    auto &overlayTransform = overlay.getComponent<Transform>();
    auto &overlaySprite = overlay.getComponent<Sprite>();

    float baseX = overlayTransform.position.x;
    float baseY = overlayTransform.position.y;

    auto &closeButton = world.createEntity();
    auto &closeTransform = closeButton.addComponent<Transform>(Vector2D(baseX + overlaySprite.dst.w - 40, baseY + 10),
                                                               0.0f, 1.0f);

    SDL_Texture *text = TextureManager::load("../asset/ui/close.png");
    SDL_FRect closeSrc{0, 0, 32, 32};
    SDL_FRect closeDest{closeTransform.position.x, closeTransform.position.y, closeSrc.w, closeSrc.h};
    closeButton.addComponent<Sprite>(text, closeSrc, closeDest, RenderLayer::UI, false);
    closeButton.addComponent<Collider>("ui", closeDest);

    auto &clickable = closeButton.addComponent<Clickable>();

    clickable.onPressed = [&closeTransform] {
        closeTransform.scale = 0.5f; // explicitly type float?
    };

    clickable.onReleased = [this, &overlay, &closeTransform] {
        closeTransform.scale = 1.0f;
        toggleSettingsOverlayVisibility(overlay);
    };

    clickable.onCancel = [&closeTransform] {
        closeTransform.scale = 1.0f;
    };

    closeButton.addComponent<Parent>(&overlay);
    auto &parentChildren = overlay.getComponent<Children>();
    parentChildren.children.push_back(&closeButton);
};

void Scene::toggleSettingsOverlayVisibility(Entity &overlay) {
    auto &sprite = overlay.getComponent<Sprite>();
    bool newVisibility = !sprite.visible;
    sprite.visible = newVisibility;

    if (overlay.hasComponent<Children>()) {
        auto &c = overlay.getComponent<Children>();

        for (auto &child: c.children) {
            if (child && child->hasComponent<Sprite>()) {
                child->getComponent<Sprite>().visible = newVisibility;
            }
            if (child && child->hasComponent<Collider>()) {
                child->getComponent<Collider>().enabled = newVisibility;
            }
        }
    }
};

Entity &Scene::createPlayerEntity(const char *spawnLocationName, const char *spritePath) {
    auto &player(world.createEntity());
    auto spawnPoint = world.getMap().mapProps.playerSpawns.find(spawnLocationName)->second;
    spawnPoint.position.x -= 64.0f / 2.0f;
    spawnPoint.position.y -= 64.0f / 2.0f;
    auto &playerTransform(player.addComponent<Transform>(spawnPoint.position, 0.0f, 1.0f));
    playerTransform.oldPosition = playerTransform.position;

    // Create player's stats
    player.addComponent<RigidBody>(240.0f, 240.0f);
    player.addComponent<Velocity>(Vector2D(0.0f, 0.0f), spawnPoint.direction, 0.0f);
    player.addComponent<Health>(Game::gameState.playerHealth);
    player.addComponent<BallHolder>();

    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture *tex = TextureManager::load(spritePath);
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst{playerTransform.position.x, playerTransform.position.y, 64, 64};
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
    SDL_FRect iconSrc{0, 0, 80, 80};
    SDL_FRect iconDest{iconTransform.position.x, iconTransform.position.y, 80, 80};
    auto& iconSpite = playerIcon.addComponent<Sprite>(iconTex, iconSrc, iconDest);
    iconSpite.renderLayer = RenderLayer::UI;
    iconSpite.visible = true;

    return playerIcon;
}

Entity &Scene::createPlayerTitleLabel(Entity &entity, int windowWidth, int windowHeight) {
    auto& playerTitleLabel(world.createEntity());
    Label label;

    if (entity.hasComponent<Player1Tag>()) {
        label = {
            "Player 1 title",
            AssetManager::getFont("fira"),
            {255, 255, 255, 255},
            LabelType::PlayerTitle,
            "player1Title"
        };
    } else {
        label = {
            "Player 2 title",
            AssetManager::getFont("fira"),
            {255, 255, 255, 255},
            LabelType::PlayerTitle,
            "player2Title"
        };

    }

    TextureManager::loadLabel(label);
    playerTitleLabel.addComponent<Label>(label);

    if (entity.hasComponent<Player1Tag>()) {
        playerTitleLabel.addComponent<Transform>(Vector2D(105, 10), 0.0f, 1.0f);
    } else {
        playerTitleLabel.addComponent<Transform>(Vector2D(windowWidth-PLAYER2_UI_Offset+15, 10), 0.0f, 1.0f);
    }

    return playerTitleLabel;

}

Entity& Scene::createPlayerLivesLabel(Entity& entity, int windowWidth, int windowHeight) {

    auto& playerLivesLabel(world.createEntity());
    Label label;

    if (entity.hasComponent<Player1Tag>()) {
        label = {
            "Player 1 lives ",
            AssetManager::getFont("arial"),
            {255, 255, 255, 255},
            LabelType::Lives,
            "player1Lives"
        };
    } else {
        label = {
            "Player 1 lives",
            AssetManager::getFont("arial"),
            {255, 255, 255, 255},
            LabelType::Lives,
            "player2Lives"
        };
    }

    TextureManager::loadLabel(label);
    playerLivesLabel.addComponent<Label>(label);

    if (entity.hasComponent<Player1Tag>()) {
        playerLivesLabel.addComponent<Transform>(Vector2D(105, 45), 0.0f, 1.0f);
    } else {
        playerLivesLabel.addComponent<Transform>(Vector2D(windowWidth-PLAYER2_UI_Offset+15, 45), 0.0f, 1.0f);
    }

    return playerLivesLabel;

}

Entity& Scene::createPlayerYarnballsLabel(Entity& entity, int windowWidth, int windowHeight) {

    auto& playerYarnballsLabel(world.createEntity());
    Label label;

    if (entity.hasComponent<Player1Tag>()) {
        label = {
            "Player 1 yarnballs ",
            AssetManager::getFont("arial"),
            {255, 255, 255, 255},
            LabelType::Yarnballs,
            "player1Yarnballs"
        };
    } else {
        label = {
            "Player 2 yarnballs ",
            AssetManager::getFont("arial"),
            {255, 255, 255, 255},
            LabelType::Yarnballs,
            "player2Yarnballs"
        };
    }

    TextureManager::loadLabel(label);
    playerYarnballsLabel.addComponent<Label>(label);

    if (entity.hasComponent<Player1Tag>()) {
        playerYarnballsLabel.addComponent<Transform>(Vector2D(105, 75), 0.0f, 1.0f);
    } else {
        playerYarnballsLabel.addComponent<Transform>(Vector2D(windowWidth-PLAYER2_UI_Offset+15, 75), 0.0f, 1.0f);
    }

    return playerYarnballsLabel;

}

void Scene::createPickupSpawner(float interval) {
    auto &ballSpawner(world.createEntity());
    ballSpawner.addComponent<TimedSpawner>(interval, [this] {
        if (Game::gameState.availableBallsForSpawning <= 0) return;
        Game::gameState.availableBallsForSpawning -= 1;
        auto &e(world.createDeferredEntity());
        // Get a valid spawn point from the side
        SpawnPoint *spawn = nullptr;
        auto &pickupSpawns = Game::gameState.spawnBallOnPlayer1Side ? world.getMap().mapProps.player1PickupSpawns : world.getMap().mapProps.player2PickupSpawns;
        while (!spawn) {
            int index = rand() % pickupSpawns.size();
            if (!pickupSpawns[index].isUsed) {
                pickupSpawns[index].isUsed = true;
                spawn = &pickupSpawns[index];
            }
        }
        int spriteSize = 64;
        int ballSize = 32;
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
            int spriteSize = 50;
            int ballSize = 25;
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

