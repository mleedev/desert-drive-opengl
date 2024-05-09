//
// Created by Matthew Halderman on 5/9/24.
//

#ifndef MATTSQUARED_GRAPHICS_VEHICLE_H
#define MATTSQUARED_GRAPHICS_VEHICLE_H


#include <memory>
#include "Object3D.h"
#include "UserInput.h"

class Vehicle {
private:
    glm::vec3 inputDirection;
    UserInput& userInput;
    Object3D& body;
public:
    glm::vec3 frontLookat;
    glm::vec3 rearCamera;
    glm::vec3 velocity;
    glm::vec3 angVelocity;
    glm::vec3 rotation;
    glm::vec3 direction;
    glm::vec3 headlightPos;
    Vehicle(Object3D &body, UserInput& userInput);
    void Update(float deltaTime);
};


#endif //MATTSQUARED_GRAPHICS_VEHICLE_H
