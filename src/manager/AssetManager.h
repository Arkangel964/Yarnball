//
// Created by WillO on 2025-10-14.
//

#ifndef TUTORIAL1_ASSETMANAGER_H
#define TUTORIAL1_ASSETMANAGER_H
#include <string>
#include <unordered_map>

#include "Component.h"

class AssetManager {
    static std::unordered_map<std::string, Animation>  animations;
    static Animation loadAnimationFromXML(const char* path);
    public:
    static SDL_AudioDeviceID audioDevice;
    static SDL_AudioSpec deviceSpec;
    static void loadAnimation(const std::string& clipName, const char* path);
    static void loadAudioDevice(SDL_AudioDeviceID device, SDL_AudioSpec spec) {
        audioDevice = device;
        deviceSpec = spec;
    };
    static const Animation& getAnimation(const std::string& clipName);
};

#endif //TUTORIAL1_ASSETMANAGER_H