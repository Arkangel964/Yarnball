cmake_minimum_required(VERSION 4.0)
project(8552Game)

set(CMAKE_CXX_STANDARD 20)

#Set SDL3 include and lib paths
if(WIN32)
    set(SDL3_PATH "${PROJECT_SOURCE_DIR}/libs/sdl3")
    set(SDL3_IMAGE_PATH "${PROJECT_SOURCE_DIR}/libs/sdl3-image")
    include_directories(
            ${SDL3_PATH}/include
            ${SDL3_IMAGE_PATH}/include
            ${PROJECT_SOURCE_DIR}/src
            ${PROJECT_SOURCE_DIR}/src/utils
            ${PROJECT_SOURCE_DIR}/src/ecs
            ${PROJECT_SOURCE_DIR}/src/ecs/system
            ${PROJECT_SOURCE_DIR}/src/vendor
    )
    link_directories(
            ${SDL3_PATH}/lib
            ${SDL3_IMAGE_PATH}/lib
    )
endif()

add_executable(8552Game src/main.cpp
        src/Game.cpp
        src/TextureManager.h
        src/TextureManager.cpp
        src/Map.h
        src/Map.cpp
        src/GameObject.h
        src/GameObject.cpp
        src/ecs/ComponentType.h
        src/ecs/Entity.h
        src/ecs/Component.h
        src/ecs/system/MovementSystem.h
        src/ecs/system/RenderSystem.h
        src/ecs/World.h
        src/utils/Vector2D.h
        src/ecs/system/KeyboardInputSystem.h
        src/utils/Vector2D.cpp
        src/utils/Collision.h
        src/utils/Collision.cpp
        src/ecs/system/CollisionSystem.h
        src/ecs/system/CollisionSystem.cpp
        src/ecs/event/EventManager.h
        src/vendor/tinyxml2.cpp
        src/ecs/system/AnimationClip.h
        src/manager/AssetManager.h
        src/manager/AssetManager.cpp
        src/ecs/system/AnimationSystem.h
        src/ecs/system/CameraSystem.h
        src/ecs/system/SpawnTimerSystem.h
        src/ecs/system/DestructionSystem.h
        src/scene/Scene.h
        src/scene/Scene.cpp
        src/manager/SceneManager.h
        src/ecs/event/BaseEvent.h
        src/ecs/system/EventResponseSystem.h
        src/ecs/system/EventResponseSystem.cpp
        src/scene/SceneType.h
        src/ecs/system/MainMenuSystem.h
        src/ecs/system/MainMenuSystem.cpp
        src/utils/RenderUtils.h
        src/ecs/system/UIRenderSystem.h
        src/ecs/system/MouseInputSystem.h
        src/ecs/system/MouseInputSystem.cpp
        src/ecs/system/AudioSystem.h
        src/ecs/system/AudioSystem.cpp
)

target_link_libraries(8552Game
        SDL3
        SDL3_image
)

if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_PATH}/bin/SDL3.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_IMAGE_PATH}/bin/SDL3_image.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif()