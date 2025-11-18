//
// Created by WillO on 2025-11-18.
//

#ifndef INC_8552GAME_AUDIOMANAGER_H
#define INC_8552GAME_AUDIOMANAGER_H

#include <string>
#include <unordered_map>

#include "SDL3_mixer/SDL_mixer.h"

class AudioManager {
    MIX_Mixer *mixer = nullptr;
    MIX_Track *musicTrack = nullptr;
    static MIX_Track *sfxTrack;
    static std::unordered_map<std::string, MIX_Audio*> audio;
    public:
    AudioManager();
    void loadAudio(const std::string& clipName, const char* path) const;
    void playMusic(const std::string& clipName) const;
    void stopMusic() const;
    static void playSfx(const std::string& clipName);
};

#endif //INC_8552GAME_AUDIOMANAGER_H