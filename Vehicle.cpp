//
// Created by Matthew Halderman on 5/9/24.
//

#include <iostream>
#include "Vehicle.h"
#include "Object3D.h"

Vehicle::Vehicle(Object3D& body,UserInput& userInput) : body(body), userInput(userInput) {
    this->body = body;
    this->userInput = userInput;
    rotation = glm::vec3(0,0,0);
}

glm::vec3 rotationToDirection(const glm::vec3& rotation) {
    glm::vec3 direction;
    direction.x = cos(rotation.y) * cos(rotation.x);
    direction.y = sin(rotation.x);
    direction.z = sin(rotation.y) * cos(rotation.x);
    return glm::normalize(direction);
}

void Vehicle::Update(float dt) { // deltaTime
    inputDirection = glm::vec3(userInput.sideInput, 0,userInput.forwardInput);
    rotation += glm::vec3(0,inputDirection.x * abs(inputDirection.z),0) * dt * 1.0f;
    direction = rotationToDirection(rotation);
    velocity = direction * inputDirection.z * 10.0f * dt;
    body.setPosition(body.getPosition() + velocity);
    body.setOrientation(-rotation);

    frontLookat = body.getPosition() + direction * 10.0f;
    rearCamera = rearCamera + (body.getPosition() - direction * 10.0f + glm::vec3(0,3,0) - rearCamera) * std::min(dt/0.2f,1.0f);

    headlightPos = body.getPosition() + direction * 2.0f + glm::vec3(0,0,0);
}