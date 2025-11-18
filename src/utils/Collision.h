//
// Created by WillO on 2025-09-30.
//

#ifndef TUTORIAL1_COLLISION_H
#define TUTORIAL1_COLLISION_H
#include <SDL3/SDL_rect.h>

#include "Component.h"

class Collision {
public:
        //axis aligned bounding box
    static bool AABB(const SDL_FRect rectA, const SDL_FRect rectB);
    static bool AABB(const Collider& colA, const Collider& colB);

    static Vector2D getAABBCollisionNormal(Collider &colA, Collider &colB);
};

#endif //TUTORIAL1_COLLISION_H