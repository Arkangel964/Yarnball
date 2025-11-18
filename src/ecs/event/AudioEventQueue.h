//
// Created by WillO on 2025-11-18.
//

#ifndef INC_8552GAME_AUDIOEVENTQUEUE_H
#define INC_8552GAME_AUDIOEVENTQUEUE_H

#include <memory>
#include <queue>
#include <string>

#include "manager/AudioManager.h"

class AudioEvent {
    std::string name;
public:
    explicit AudioEvent(const std::string &name) : name(name){}
    void execute() const {
        AudioManager::playSfx(name);
    }
};

class AudioEventQueue {
    std::queue<std::unique_ptr<AudioEvent>> events;
public:
    void push(std::unique_ptr<AudioEvent> event) {
        events.push(std::move(event));
    }

    void process() {
        while(!events.empty()) {
            events.front()->execute();
            events.pop();
        }
    }

};

#endif //INC_8552GAME_AUDIOEVENTQUEUE_H