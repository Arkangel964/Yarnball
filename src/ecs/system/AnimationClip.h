//
// Created by WillO on 2025-10-14.
//

#ifndef TUTORIAL1_ANIMATIONCLIP_H
#define TUTORIAL1_ANIMATIONCLIP_H
#include <vector>
#include <SDL3/SDL_rect.h>

struct AnimationClip {
  std::vector<SDL_FRect> frameIndices;
};

#endif //TUTORIAL1_ANIMATIONCLIP_H