//
// Created by WillO on 2025-11-17.
//

#ifndef INC_8552GAME_AUDIOSYSTEM_H
#define INC_8552GAME_AUDIOSYSTEM_H
#include <iostream>
#include <string>
#include <vector>

#include "manager/AssetManager.h"
#include "SDL3/SDL_audio.h"
#include "SDL3/SDL_timer.h"

class World;

class AudioSystem {
private:
    std::vector<SDL_AudioStream*> activeAudioStreams{};
public:
    void PlaySoundWav(const char* path) {
        std::cout << "PlaySound" << std::endl;

        //Loading WAV file
        SDL_AudioSpec wavSpec{};
        Uint8* wavBuffer = nullptr;
        Uint32 wavBufferSize = 0;
        if(!SDL_LoadWAV(path, &wavSpec, &wavBuffer, &wavBufferSize)) {
            std::cout << "Failed to load wav" << std::endl;
        }
        //Binding audio stream to device and playing WAV over it
        SDL_AudioDeviceID id = AssetManager::audioDevice;
        SDL_AudioSpec destSpec = AssetManager::deviceSpec;
        SDL_AudioStream* audio_stream = SDL_CreateAudioStream(&wavSpec, &destSpec);
        SDL_BindAudioStream(id, audio_stream);
        SDL_PutAudioStreamData(audio_stream, wavBuffer, static_cast<int>(wavBufferSize));
        activeAudioStreams.push_back(audio_stream);
        SDL_free(wavBuffer);
    }

    void update(World& world);
};

#endif //INC_8552GAME_AUDIOSYSTEM_H