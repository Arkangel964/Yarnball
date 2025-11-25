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

    //Create all coins
    for (auto &itemPos: world.getMap().itemPositions) {
        auto &item(world.createEntity());
        item.addComponent<Transform>(Vector2D(itemPos.x, itemPos.y), 0.0f, 1.0f);
        SDL_Texture *itemTex = TextureManager::load("../asset/coin.png");
        SDL_FRect itemSrc(0, 0, 32, 32);
        SDL_FRect itemDest(itemPos.x, itemPos.y, 32, 32);
        item.addComponent<Sprite>(itemTex, itemSrc, itemDest);
        auto &itemCollider = item.addComponent<Collider>("item");
        itemCollider.rect.w = itemDest.w;
        itemCollider.rect.h = itemDest.h;
    }

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

    //Load the camera
    auto &cam = world.createEntity();
    SDL_FRect camView{};
    camView.x = 0;
    camView.y = 0;
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, world.getMap().width * 32, world.getMap().height * 32);

    //reset balls available for spawning
    Game::gameState.availableBallsForSpawning = 6;

    //Create the player1
    auto &player1(world.createEntity());
    auto &player1Transform(player1.addComponent<Transform>(Vector2D(0, 0), 0.0f, 1.0f));
    player1Transform.oldPosition = player1Transform.position;

    // Create player 1's stats
    player1.addComponent<RigidBody>(240.0f, 240.0f);
    player1.addComponent<Health>(Game::gameState.playerHealth);
    player1.addComponent<BallHolder>();

    Animation anim = AssetManager::getAnimation("player");
    player1.addComponent<Animation>(anim);

    SDL_Texture *tex = TextureManager::load("../asset/animations/cat_brown_anim.png");
    SDL_FRect player1Src = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect player1Dst{player1Transform.position.x, player1Transform.position.y, 64, 64};
    player1.addComponent<Sprite>(tex, player1Src, player1Dst);
    auto &player1Collider = player1.addComponent<Collider>("player");
    player1Collider.rect.w = player1Dst.w;
    player1Collider.rect.h = player1Dst.h;

    player1.addComponent<Keybinds>(SDLK_W, SDLK_S, SDLK_A, SDLK_D, SDLK_E);

    player1.addComponent<Player1Tag>();

    //Create the player2
    auto &player2(world.createEntity());
    auto &player2Transform(player2.addComponent<Transform>(Vector2D(0, 64), 0.0f, 1.0f));
    player2Transform.oldPosition = player2Transform.position;

    // Create player 2's stats
    player2.addComponent<RigidBody>(240.0f, 240.0f);
    player2.addComponent<Health>(Game::gameState.playerHealth);
    player2.addComponent<BallHolder>();

    anim = AssetManager::getAnimation("player");
    player2.addComponent<Animation>(anim);

    tex = TextureManager::load("../asset/animations/cat_grey_anim.png");
    SDL_FRect player2Src = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect player2Dst{player2Transform.position.x, player2Transform.position.y, 64, 64};
    player2.addComponent<Sprite>(tex, player2Src, player2Dst);
    auto &player2Collider = player2.addComponent<Collider>("player");
    player2Collider.rect.w = player2Dst.w;
    player2Collider.rect.h = player2Dst.h;

    player2.addComponent<Keybinds>(SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_RSHIFT);

    player2.addComponent<Player2Tag>();

    // Create player 1's portrait
    auto &p1Icon(world.createEntity());
    auto p1IconTransform = p1Icon.addComponent<Transform>(Vector2D(10, 15), 0.0f, 1.0f);

    SDL_Texture *p1IconTex = TextureManager::load("../asset/ui/player1icon.png");
    SDL_FRect p1IconSrc{0, 0, 80, 80};
    SDL_FRect p1IconDest{p1IconTransform.position.x, p1IconTransform.position.y, 80, 80};
    auto& p1IconSpite = p1Icon.addComponent<Sprite>(p1IconTex, p1IconSrc, p1IconDest);
    p1IconSpite.renderLayer = RenderLayer::UI;
    p1IconSpite.visible = true;

    createPlayerTitleLabel(player1, windowWidth, windowHeight);
    createPlayerLivesLabel(player1, windowWidth, windowHeight);
    createPlayerYarnballsLabel(player1, windowWidth, windowHeight);

    // Create player 2's portrait
    auto &p2Icon(world.createEntity());
    auto p2IconTransform = p2Icon.addComponent<Transform>(Vector2D(windowWidth-PLAYER2_UI_Offset-80, 15), 0.0f, 1.0f);

    SDL_Texture *p2IconTex = TextureManager::load("../asset/ui/player2icon.png");
    SDL_FRect p2IconSrc{0, 0, 80, 80};
    SDL_FRect p2IconDest{p2IconTransform.position.x, p2IconTransform.position.y, 80, 80};
    auto& p2IconSpite = p2Icon.addComponent<Sprite>(p2IconTex, p2IconSrc, p2IconDest);
    p2IconSpite.renderLayer = RenderLayer::UI;
    p2IconSpite.visible = true;

    createPlayerTitleLabel(player2, windowWidth, windowHeight);
    createPlayerLivesLabel(player2, windowWidth, windowHeight);
    createPlayerYarnballsLabel(player2, windowWidth, windowHeight);

    auto &spawner(world.createEntity());
    Transform t = spawner.addComponent<Transform>(Vector2D(windowWidth / 2, windowHeight / 2), 0.0f, 1.0f);
    spawner.addComponent<TimedSpawner>(2.0f, [this, t] {
        //create our projectile (bird)
        auto &e(world.createDeferredEntity());
        e.addComponent<Transform>(Vector2D(t.position.x, t.position.y), 0.0f, 1.0f);
//        e.addComponent<Velocity>(Vector2D(0, -1), Vector2D(0, -1), 100.0f);
        e.addComponent<RigidBody>(100.0f, 0.25f);
        PhysicsSystem::addImpulse(e, Vector2D(0, -1), 100.0f);

        Animation anim = AssetManager::getAnimation("enemy");
        e.addComponent<Animation>(anim);

        SDL_Texture *tex = TextureManager::load("../asset/animations/bird_anim.png");
        SDL_FRect animSrc(0, 0, 32, 32);
        SDL_FRect dest(t.position.x, t.position.y, 32, 32);
        e.addComponent<Sprite>(tex, animSrc, dest);

        auto &c = e.addComponent<Collider>("projectile");
        c.rect.w = dest.w;
        c.rect.h = dest.h;

        e.addComponent<ProjectileTag>();
        e.addComponent<DestroyOnStop>();
    });

    //dodgeball spawner
    auto &ballSpawner(world.createEntity());
    Transform ballSpawnTransform = ballSpawner.addComponent<Transform>(Vector2D(0, 0), 0.0f, 1.0f);
    ballSpawner.addComponent<TimedSpawner>(5.0f, [this, ballSpawnTransform, windowWidth, windowHeight] {
        if (Game::gameState.availableBallsForSpawning > 0) {
            Game::gameState.availableBallsForSpawning -= 1;
            //create our projectile (ball)
            auto &e(world.createDeferredEntity());

            //https://stackoverflow.com/questions/686353/random-float-number-generation
            float ballTransformX = ballSpawnTransform.position.x + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(windowWidth -ballSpawnTransform.position.x)));
            float ballTransformY = ballSpawnTransform.position.y + static_cast<float>(rand())/(static_cast<float>(RAND_MAX/(windowHeight -ballSpawnTransform.position.y)));

            e.addComponent<Transform>(Vector2D(ballTransformX, ballTransformY), 0.0f, 1.0f);

            SDL_Texture *tex = TextureManager::load("../asset/ball.png");
            SDL_FRect spriteSrc(0, 0, 32, 32);
            SDL_FRect dest(ballSpawnTransform.position.x, ballSpawnTransform.position.y, 32, 32);
            e.addComponent<Sprite>(tex, spriteSrc, dest);

            auto &c = e.addComponent<Collider>("inactiveBall");
            c.rect.w = dest.w;
            c.rect.h = dest.h;
        }

    });

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

