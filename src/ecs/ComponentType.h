//
// Created by WillO on 2025-09-16.
//

#ifndef TUTORIAL1_COMPONENTTYPE_H
#define TUTORIAL1_COMPONENTTYPE_H
#include <cstddef>

//type alias for size_t, designed to represent sizes and indices because it is unsigned and large
using ComponentTypeID = std::size_t;

//when you have a free function defined in a header, each .cpp file that includes this header
//would get its own definition, causing a linker error. using inline merges them into one def
//this is a global counter generator, returns unique number every time it is called
inline ComponentTypeID getComponentTypeID() {
    static ComponentTypeID id = 0;
    return id++;
}

//overloads above function, they have different signatures
//template allows us to use generic typing, and is inline by default
//this allows for GetComponentTypeID<Position> -> always == 0
//a static local variable is created and initialized once, so the ID of a given component will always be the same
template <typename T> ComponentTypeID getComponentTypeID() {
    static ComponentTypeID typeID = getComponentTypeID();
    return typeID;
}

#endif //TUTORIAL1_COMPONENTTYPE_H
