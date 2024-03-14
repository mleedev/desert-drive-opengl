cmake_minimum_required(VERSION 3.27)
project(textureBasics)

set(CMAKE_CXX_STANDARD 17)

add_executable(textureBasics
        main.cpp
        glad.c
        Mesh3D.cpp
        Object3D.cpp
        ShaderProgram.cpp
        AssimpImport.cpp
        StbImage.cpp
)

find_package(SFML COMPONENTS system window REQUIRED)
find_package(GLM CONFIG REQUIRED)
find_package(ASSIMP REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/include)

target_link_libraries(textureBasics
        sfml-system sfml-window
        ${ASSIMP_LIBRARIES}
)