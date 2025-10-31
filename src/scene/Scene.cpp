#include "Scene.h"

#include "../manager/AssetManager.h"
#include "Game.h"


Scene::Scene(SceneType sceneType, const char* sceneName, const char* mapPath, const int windowWidth, const int windowHeight) : name(sceneName), type(sceneType) {

    if (sceneType == SceneType::MainMenu) {
        //camera
        auto& cam = world.createEntity();
        cam.addComponent<Camera>();

        //menu
        auto &menu(world.createEntity());
        auto menuTransform = menu.addComponent<Transform>(Vector2D(0,0), 0.0f, 1.0f);
        SDL_Texture* menuTex = TextureManager::load("../asset/menu.png");
        SDL_FRect menuSrc{0,0,800,600};
        SDL_FRect menuDest{menuTransform.position.x, menuTransform.position.y, (float)windowWidth, (float)windowHeight};
        menu.addComponent<Sprite>(menuTex, menuSrc, menuDest);
        return;
    }

    //load map
    world.getMap().load(mapPath, TextureManager::load("../asset/spritesheet.png"));

    //Create all coins
    for (auto& itemPos : world.getMap().itemPositions) {
        auto& item(world.createEntity());
        item.addComponent<Transform>(Vector2D(itemPos.x, itemPos.y), 0.0f, 1.0f);
        SDL_Texture* itemTex = TextureManager::load("../asset/coin.png");
        SDL_FRect itemSrc(0, 0, 32, 32);
        SDL_FRect itemDest(itemPos.x, itemPos.y, 32, 32);
        item.addComponent<Sprite>(itemTex, itemSrc, itemDest);
        auto& itemCollider = item.addComponent<Collider>("item");
        itemCollider.rect.w = itemDest.w;
        itemCollider.rect.h = itemDest.h;
    }

    for (auto& collider : world.getMap().wallColliders) {
        auto& e = world.createEntity();
        e.addComponent<Transform>(Vector2D(collider.rect.x, collider.rect.y), 0.0f, 1.0f);
        auto& coll = e.addComponent<Collider>("wall");
        coll.rect.x = collider.rect.x;
        coll.rect.y = collider.rect.y;
        coll.rect.w = collider.rect.w;
        coll.rect.h = collider.rect.h;

        SDL_Texture* tex = TextureManager::load("../asset/spritesheet.png");
        SDL_FRect colSrc {0, 32, 32, 32};
        SDL_FRect colDst {coll.rect.x, coll.rect.y, coll.rect.w, coll.rect.h};
        e.addComponent<Sprite>(tex, colSrc, colDst);
    }

    //Load the camera
    auto& cam = world.createEntity();
    SDL_FRect camView{};
    camView.x = 0;
    camView.y = 0;
    camView.w = windowWidth;
    camView.h = windowHeight;
    cam.addComponent<Camera>(camView, world.getMap().width * 32, world.getMap().height * 32);


    //Create the player
    auto& player(world.createEntity());
    auto& playerTransform(player.addComponent<Transform>(Vector2D(0,0), 0.0f, 1.0f));
    player.addComponent<Velocity>(Vector2D(0,0), Vector2D(0,0), 240.0f);
    player.addComponent<Health>(Game::gameState.playerHealth);

    Animation anim = AssetManager::getAnimation("player");
    player.addComponent<Animation>(anim);

    SDL_Texture* tex = TextureManager::load("../asset/animations/bull_anim.png");
    SDL_FRect playerSrc = anim.clips[anim.currentClip].frameIndices[0];
    SDL_FRect playerDst {playerTransform.position.x, playerTransform.position.y, 64, 64};
    player.addComponent<Sprite>(tex, playerSrc, playerDst);
    auto& playerCollider = player.addComponent<Collider>("player");
    playerCollider.rect.w = playerDst.w;
    playerCollider.rect.h = playerDst.h;

    player.addComponent<PlayerTag>();

    auto& spawner(world.createEntity());
    Transform t = spawner.addComponent<Transform>(Vector2D(windowWidth/2,windowHeight - 5), 0.0f, 1.0f);
    spawner.addComponent<TimedSpawner>(2.0f, [this, t] {
        //create our projectile (bird)
        auto& e(world.createDeferredEntity());
        e.addComponent<Transform>(Vector2D(t.position.x, t.position.y), 0.0f, 1.0f);
        e.addComponent<Velocity>(Vector2D(0,-1), Vector2D(0,-1), 100.0f);

        Animation anim = AssetManager::getAnimation("enemy");
        e.addComponent<Animation>(anim);

        SDL_Texture* tex = TextureManager::load("../asset/animations/bird_anim.png");
        SDL_FRect animSrc(0, 0, 32, 32);
        SDL_FRect dest(t.position.x, t.position.y, 32, 32);
        e.addComponent<Sprite>(tex, animSrc, dest);

        Collider c = e.addComponent<Collider>("projectile");
        c.rect.w = dest.w;
        c.rect.h = dest.h;

        e.addComponent<ProjectileTag>();
    });

    //add scene state
    auto& state(world.createEntity());
    state.addComponent<SceneState>();
}
