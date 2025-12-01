//
// Created by WillO on 2025-11-18.
//

#include "AudioManager.h"

#include <iostream>
#include <ostream>

std::unordered_map<std::string, MIX_Audio*> AudioManager::audio;

MIX_Mixer* AudioManager::mixer;
std::vector<MIX_Track*> AudioManager::SFXtracks;

AudioManager::AudioManager() {
    if (MIX_Init() == 0) {
        std::cout << "MIX_Init failed" << std::endl;
        return;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    SFXtracks = std::vector<MIX_Track*>();
    if (!mixer) {
        std::cout << "MIX_CreateMixerDevice failed" << std::endl;
        return;
    }

    musicTrack = MIX_CreateTrack(mixer);
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

    SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    MIX_PlayTrack(musicTrack, props); //-1 means loop endlessly
    // std::cout << "Playing Music" << std::endl;
}

void AudioManager::stopMusic() const {
    MIX_StopTrack(musicTrack, 10);
}

void AudioManager::playSfx(const std::string &clipName) {
    MIX_Track *sfxTrack = nullptr;
    for (auto &track : SFXtracks) {
        if (MIX_GetTrackRemaining(track) <= 0) {
            sfxTrack = track;
        }
    }
    if (sfxTrack == nullptr) {
        std::cout << "No available sfx tracks, creating new..." << std::endl;
        sfxTrack = MIX_CreateTrack(mixer);
        SFXtracks.emplace_back(sfxTrack);
    }

    if (MIX_SetTrackAudio(sfxTrack, audio[clipName]) == 0) {
        std::cout << "MIX_SetTrackAudio failed" << std::endl;
        return;
    }

    MIX_PlayTrack(sfxTrack, 0); //no loop
    // std::cout << "Playing SFX" << std::endl;
}

