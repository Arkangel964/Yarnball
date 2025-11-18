//
// Created by WillO on 2025-09-16.
//

#ifndef TUTORIAL1_ENTITY_H
#define TUTORIAL1_ENTITY_H
#include <bitset>
#include <cstddef>
#include <array>

#include "ComponentType.h"

//create a const that is evaluated at compile time, 32 components fits perfectly in a 32-bit integer
constexpr std::size_t MAX_COMPONENTS = 32;

//bitset is just an array of bits, basically
//bit operations are fast and good for checking each frame
using ComponentBitSet = std::bitset<MAX_COMPONENTS>;

//fixed size array, where each index is a component
//the index corresponds to the ComponentTypeID
//we use a fixed number of components, so there is no dynamic allocation needed (efficient!)
using ComponentArray = std::array<void*, MAX_COMPONENTS>;

class Entity {
    bool active = true;
    ComponentArray componentArray{};
    ComponentBitSet componentBitSet{};

public:
    bool isActive() { return active; }
    void destroy() { active = false; };

    //templates have to be implemented in header
    //check if the entity has a component
    template<typename T> bool hasComponent() {
        return componentBitSet[getComponentTypeID<T>()];
    }

    //template that receives multiple arguments
    //entity.addComponent<Position>(x, y)
    //entity.addComponent<Health>(value)
    template<typename T, typename... mArgs> T& addComponent(mArgs&&... args) {
        //implements perfect forwarding, everything is not treated as an lValue
        //this is more efficient with moves and temporaries
        T* component(new T(std::forward<mArgs>(args)...));

        //add the new component to the array
        componentArray[getComponentTypeID<T>()] = component;
        //update the bitset to true so that it knows it has the component
        componentBitSet[getComponentTypeID<T>()] = true;
        return *component;
    }

    template<typename T> void removeComponent() {
        // Remove the component from the array and memory
        delete static_cast<T*>(componentArray[getComponentTypeID<T>()]);
        componentArray[getComponentTypeID<T>()] = nullptr;
        // Update the bitset to false so that it is removed entirely
        componentBitSet[getComponentTypeID<T>()] = false;
    }

    template<typename T> T& getComponent() {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }

    template<typename T> void deactivateComponent() {
        componentBitSet[getComponentTypeID<T>()] = false;
    }
};

#endif //TUTORIAL1_ENTITY_H
