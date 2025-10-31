//
// Created by WillO on 2025-09-30.
//
#include "Collision.h"

//Axis aligned bounding box
bool Collision::AABB(const SDL_FRect rectA, const SDL_FRect rectB) {
    //is the right edge of rect A >= left edge of rect B
    //is the right edge of rect B >= left edge of rect A
    //then do the same for bottom and top of both
    if (rectA.x + rectA.w > rectB.x &&
        rectB.x + rectB.w > rectA.x &&
        rectA.y + rectA.h > rectB.y &&
        rectB.y + rectB.h > rectA.y) {
        return true;
    }
    return false;
}

//passing in a collider
bool Collision::AABB(const Collider& colA, const Collider& colB) {
    return AABB(colA.rect, colB.rect);
}