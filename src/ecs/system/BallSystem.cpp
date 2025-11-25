//
// Created by WillO on 2025-11-24.
//

#include "BallSystem.h"
#include "World.h"
#include "Game.h"

void BallSystem::update(World &world, float dt) {
    for (auto& entity : world.getEntities()) {
        if (entity->hasComponent<BallHolder>() && entity->hasComponent<Velocity>() && entity->hasComponent<Transform>()) {
            auto& b = entity->getComponent<BallHolder>();
            auto& v = entity->getComponent<Velocity>();
            auto& t = entity->getComponent<Transform>();
            auto& spr = entity->getComponent<Sprite>();

            b.cooldown -= dt;

            if (b.shouldThrowBall == true) {
                b.shouldThrowBall = false;
                cout << "Checking if ball can be thrown" << endl;
                if (b.numBallsHeld > 0 && b.cooldown <= 0.0f) {
                    cout << "Throwing ball" << endl;
                    b.numBallsHeld--;
                    b.cooldown = 0.2f;
                    Game::gameState.availableBallsForSpawning += 1;

                    //create the ball
                    auto &ball(world.createDeferredEntity());

                    SDL_Texture *tex = TextureManager::load("../asset/ball.png");
                    SDL_FRect animSrc(0, 0, 32, 32);
                    SDL_FRect dest(t.position.x, t.position.y, 32, 32);
                    ball.addComponent<Sprite>(tex, animSrc, dest);

                    float middleX = t.position.x + (spr.dst.w / 2) - (ball.getComponent<Sprite>().dst.w/2);
                    float middleY = t.position.y + (spr.dst.h / 2) - (ball.getComponent<Sprite>().dst.h/2);
                    ball.addComponent<Transform>(Vector2D(middleX+(v.oldDirection.x * ballSpawnDistance), middleY+(v.oldDirection.y * ballSpawnDistance)), 0.0f, 1.0f);
                    ball.addComponent<RigidBody>(500.0f, 3.0f);
                    PhysicsSystem::addImpulse(ball, Vector2D(v.oldDirection.x, v.oldDirection.y), 500.0f);


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
