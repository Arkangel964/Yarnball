//
// Created by WillO on 2025-11-24.
//

#include "BallSystem.h"
#include "World.h"
#include "Game.h"

void BallSystem::update(World &world) {
    for (auto& entity : world.getEntities()) {
        if (entity->hasComponent<BallHolder>() && entity->hasComponent<Velocity>() && entity->hasComponent<Transform>()) {
            auto& b = entity->getComponent<BallHolder>();
            auto& v = entity->getComponent<Velocity>();
            auto& t = entity->getComponent<Transform>();

            if (b.shouldThrowBall == true) {
                b.shouldThrowBall = false;
                cout << "Checking if ball can be thrown" << endl;
                if (b.holdingBall) {
                    cout << "Throwing ball" << endl;
                    b.holdingBall = false;
                    Game::gameState.availableBallsForSpawning += 1;

                    //create the ball
                    auto &ball(world.createDeferredEntity());

                    ball.addComponent<Transform>(Vector2D(t.position.x+(v.oldDirection.x * ballSpawnDistance), t.position.y+(v.oldDirection.y * ballSpawnDistance)), 0.0f, 1.0f);
                    ball.addComponent<RigidBody>(500.0f, 3.0f);
                    PhysicsSystem::addImpulse(ball, Vector2D(v.oldDirection.x, v.oldDirection.y), 500.0f);

                    SDL_Texture *tex = TextureManager::load("../asset/ball.png");
                    SDL_FRect animSrc(0, 0, 32, 32);
                    SDL_FRect dest(t.position.x, t.position.y, 32, 32);
                    ball.addComponent<Sprite>(tex, animSrc, dest);

                    auto &c = ball.addComponent<Collider>("projectile");
                    c.rect.w = dest.w;
                    c.rect.h = dest.h;

                    ball.addComponent<ProjectileTag>();
                    ball.addComponent<DestroyOnStop>();
                    if (entity->hasComponent<Player1Tag>()) {
                        ball.addComponent<Ball>(1);
                    } else if (entity->hasComponent<Player2Tag>()) {
                        ball.addComponent<Ball>(2);
                    }
                }
            }
        }
    }
}
