#ifndef MATTSQUARED_GRAPHICS_SCENE_H
#define MATTSQUARED_GRAPHICS_SCENE_H


#include <vector>
#include "ShaderProgram.h"
#include "Object3D.h"
#include "Animator.h"

class Scene {
public:
    ShaderProgram defaultShader;
    std::vector<Object3D> objects;
    std::vector<Animator> animators;

    Scene(ShaderProgram&& defaultShader, std::vector<Object3D>&& objects, std::vector<Animator>&& animators);
    Scene(ShaderProgram shader, const Object3D& object);

    static Scene lifeOfPi();
};


#endif //MATTSQUARED_GRAPHICS_SCENE_H
