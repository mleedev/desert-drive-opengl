cmake_minimum_required(VERSION 3.27)
project(mattsquared_graphics)

set(CMAKE_CXX_STANDARD 17)

add_executable(mattsquared_graphics)

target_sources(mattsquared_graphics PRIVATE
        src/main.cpp
        src/glad.c
        src/Mesh3D.cpp
        src/Object3D.cpp
        src/ShaderProgram.cpp
        src/AssimpImport.cpp
        src/StbImage.cpp
        src/Animator.cpp
        src/Scene.cpp
        src/DynamicLight.cpp
        src/UserInput.cpp
        src/Vehicle.cpp
        src/SkyShading.cpp

        include/BezierAnimation.h
        include/DynamicLight.h
        include/SkyShading.h
        include/UserInput.h
        include/Vehicle.h
)

target_include_directories(mattsquared_graphics PRIVATE ${CMAKE_SOURCE_DIR}/include)

find_package(SFML COMPONENTS System Window REQUIRED)
find_package(GLM CONFIG REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(ZLIB REQUIRED)

target_link_libraries(mattsquared_graphics
        SFML::System
        SFML::Window
        ${ASSIMP_LIBRARIES}
        ZLIB::ZLIB
)
