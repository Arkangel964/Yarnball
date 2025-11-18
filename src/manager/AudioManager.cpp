//
// Created by WillO on 2025-11-18.
//

#include "AudioManager.h"

#include <iostream>
#include <ostream>

MIX_Track* AudioManager::sfxTrack;
std::unordered_map<std::string, MIX_Audio*> AudioManager::audio;

AudioManager::AudioManager() {
    if (MIX_Init() == 0) {
        std::cout << "MIX_Init failed" << std::endl;
        return;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer) {
        std::cout << "MIX_CreateMixerDevice failed" << std::endl;
        return;
    }

    musicTrack = MIX_CreateTrack(mixer);
    sfxTrack = MIX_CreateTrack(mixer);
    MIX_SetTrackGain(musicTrack, 0.8f);
}

void AudioManager::loadAudio(const std::string &clipName, const char *path) const {
    if (audio.contains(clipName)) {
        return;
    }

    auto audioPtr = MIX_LoadAudio(mixer, path, true);
    if (!audioPtr) {
        std::cout << "MIX_LoadAudio failed" << std::endl;
        return;
    }

    audio.emplace(clipName, audioPtr);
}

void AudioManager::playMusic(const std::string& clipName) const {
    if (MIX_SetTrackAudio(musicTrack, audio[clipName]) == 0) {
        std::cout << "MIX_SetTrackAudio failed" << std::endl;
        return;
    }

    MIX_PlayTrack(musicTrack, -1); //-1 means loop endlessly
    std::cout << "Playing Music" << std::endl;
}

void AudioManager::stopMusic() const {
    MIX_StopTrack(musicTrack, 10);
}

void AudioManager::playSfx(const std::string &clipName) {
    if (MIX_SetTrackAudio(sfxTrack, audio[clipName]) == 0) {
        std::cout << "MIX_SetTrackAudio failed" << std::endl;
        return;
    }

    MIX_PlayTrack(sfxTrack, 0); //no loop
    std::cout << "Playing SFX" << std::endl;
}

