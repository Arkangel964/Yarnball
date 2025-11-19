//
// Created by WillO on 2025-10-14.
//

#ifndef TUTORIAL1_ANIMATIONSYSTEM_H
#define TUTORIAL1_ANIMATIONSYSTEM_H
#include <memory>
#include <vector>

#include "Component.h"
#include "Entity.h"

//State system: Deciding which clip to use
//Check if the animation has been switched
//Playback system: advances the animation

class AnimationSystem {
public:
    void update(const std::vector<std::unique_ptr<Entity>>& entities, float dt) {
        for (auto& entity : entities) {
            if (entity->hasComponent<Animation>()) {
                auto& anim = entity->getComponent<Animation>();

                //state system
                std::string newClip;
                if (entity->hasComponent<Player1Tag>() || entity->hasComponent<Player2Tag>()) {
                    if (entity->hasComponent<Velocity>()) {
                        auto& vel = entity->getComponent<Velocity>();
                        if (vel.direction.x > 0.0f) {
                            if (vel.direction.y < -0.001f) {
                                newClip = "walk_right_up";
                            } else if (vel.direction.y > 0.001f) {
                                newClip = "walk_right_down";
                            } else {
                                newClip = "walk_right";
                            }
                        } else if (vel.direction.x < 0.0f) {
                            if (vel.direction.y < -0.001f) {
                                newClip = "walk_left_up";
                            } else if (vel.direction.y > 0.001f) {
                                newClip = "walk_left_down";
                            } else {
                                newClip = "walk_left";
                            }
                        } else if (vel.direction.y > 0.0f) {
                            newClip = "walk_down";
                        } else if (vel.direction.y < 0.0f) {
                            newClip = "walk_up";
                        } else if (vel.oldDirection.x > 0.0f) {
                            if (vel.oldDirection.y < -0.001f) {
                                newClip = "idle_right_up";
                            } else if (vel.oldDirection.y > 0.001f) {
                                newClip = "idle_right_down";
                            } else {
                                newClip = "idle_right";
                            }
                        } else if (vel.oldDirection.x < 0.0f) {
                            if (vel.oldDirection.y < -0.001f) {
                                newClip = "idle_left_up";
                            } else if (vel.oldDirection.y > 0.001f) {
                                newClip = "idle_left_down";
                            } else {
                                newClip = "idle_left";
                            }
                        } else if (vel.oldDirection.y > 0.0f) {
                            newClip = "idle_down";
                        } else if (vel.oldDirection.y < 0.0f) {
                            newClip = "idle_up";
                        } else {
                            newClip = "idle_right";
                        }
                    } else {
                        newClip = "idle_right";
                    }
                } else {
                    newClip = "idle_right";
                }


                //Check if the animation has switched
                //if the chosen clip is different from the current one, switch to new clip, reset time and frame index
                if (newClip != anim.currentClip) {
                    anim.currentClip = newClip; //switch to new clip
                    anim.time = 0.0f; //reset time
                    anim.currentFrame = 0; //reset frame index
                }

                //playback system: advance the animation
                float animFrameSpeed = anim.speed; //how long each animation frame should last
                auto clip = anim.clips[anim.currentClip]; //retrieve the frame data from the current clip
                //advance time
                anim.time += dt; //every game frame we add the accumulated time

                //once enough time has passed
                if (anim.time >= animFrameSpeed) {
                    anim.time -= animFrameSpeed;

                    std::size_t totalAnimFrames = clip.frameIndices.size();
                    //go to next anim frame, wrap around % to loop at end of clip
                    anim.currentFrame = (anim.currentFrame + 1) % totalAnimFrames;
                }


            }
        }
    }
};

#endif //TUTORIAL1_ANIMATIONSYSTEM_H