//
// Created by WillO on 2025-09-30.
//

#ifndef TUTORIAL1_EVENTMANAGER_H
#define TUTORIAL1_EVENTMANAGER_H
#include <functional>
#include <vector>

#include "BaseEvent.h"
#include "Entity.h"

using namespace std;


//Observer pattern
class EventManager {
public:
    using Handler = function<void(const BaseEvent&)>;
    /*template<typename EventType>
    void emit(const EventType& event) {
        //retrieve subscribers to certain event
        auto& listeners = getListeners<EventType>();
        //loop all subs to event
        for (auto& listener : listeners) {
            listener(event); //invoke the function here
        }
    }*/

    void emit(const BaseEvent& event) const {
        for (const auto& listener : listeners) {
            listener(event);
        }
    }

    /*template<typename EventType>
    void subscribe(std::function<void(const EventType&)> callback) {
        //pass in the callback/callable wrapper/subscription to the list of subscriptions
        getListeners<EventType>().push_back(callback);
    }*/

    void subscribe(const Handler& callback) {
        listeners.emplace_back(callback);
    }

private:
    //each event type essentially has its own std::vector of listeners without us having to manage it explicitly
    //this is done using the static local
    //std::function<void(const EventType&)> is a callable wrapper, and can hold anything that can be called like a func

    /*template<typename EventType>
    static std::vector<std::function<void(const EventType&)>>& getListeners() {
        static std::vector<std::function<void(const EventType&)>> listeners;
        return listeners;
    }*/

    vector<Handler> listeners;
};

#endif //TUTORIAL1_EVENTMANAGER_H