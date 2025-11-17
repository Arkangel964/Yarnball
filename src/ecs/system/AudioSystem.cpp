//
// Created by WillO on 2025-11-17.
//
#include "AudioSystem.h"
#include "World.h"

void AudioSystem::update(World &world)  {
    //clean up finished audio
    std::vector<SDL_AudioStream*>::iterator iter = activeAudioStreams.begin();
    while (iter != activeAudioStreams.end()) {
        if (SDL_GetAudioStreamAvailable(*iter) <= 0) {
            std::cout << "Destroying audio stream" << std::endl;
            activeAudioStreams.erase(iter);
            SDL_DestroyAudioStream(*iter);
        } else {
            ++iter;
        }
    }

    const vector<unique_ptr<Entity>>& entities = world.getEntities();
    for (auto& entity : entities) {
        if (entity->hasComponent<SoundEffect>()) {
            auto& sfx = entity->getComponent<SoundEffect>();
            PlaySoundWav(sfx.path);
            entity->destroy();
        }
    }

}
