//
// Created by Matthew Halderman on 5/9/24.
//

#include <iostream>
#include "Vehicle.h"
#include "Object3D.h"

const float maxTurnSpeed = 80.0f;
const float turnAcceleration = 320.0f;
const float maxSpeed = 20.0f;
const float acceleration = 12.0f;
const float deccelerationMult = 0.5f;
const float brakeMult = 4.0f;
const float speedTurnThreshold = 15.0f;

void Vehicle::SetLights() {
    headlights.setLightSpaceMatrix(glm::mat4(
            35,0,0,0, //Position
            1,1,0.7,0, //Color
            3,60,55,0, //LightType, Range, Cuttoff Angle (For spotlights)
            0,0,1,0 //LookAt (For spotlights)
            ));
    l_brakeLight.setLightSpaceMatrix(glm::mat4(
            35,0,0,0, //Position
            1,0.1,0.1,0, //Color
            2,8,0,0, //LightType, Range, Cuttoff Angle (For spotlights)
            0,0,0,0 //LookAt (For spotlights)
    ));
    r_brakeLight.setLightSpaceMatrix(glm::mat4(
            35,0,0,0, //Position
            1,0.1,0.1,0, //Color
            2,8,0,0, //LightType, Range, Cuttoff Angle (For spotlights)
            0,0,0,0 //LookAt (For spotlights)
    ));
}

Vehicle::Vehicle(Object3D &body, UserInput &userInput, DynamicLight &headlights, DynamicLight &lBrakeLight,
                 DynamicLight &rBrakeLight)
        : body(body), userInput(userInput), headlights(headlights), l_brakeLight(lBrakeLight),
          r_brakeLight(rBrakeLight) {
    rotation = glm::vec3(0,0,0);

    SetLights();
}

glm::vec3 rotationToDirection(const glm::vec3& rotation) {
    glm::vec3 direction;
    direction.x = cos(rotation.y) * cos(rotation.x);
    direction.y = sin(rotation.x);
    direction.z = sin(rotation.y) * cos(rotation.x);
    return glm::normalize(direction);
}

void Vehicle::accelerate(float dt) {
    speed += acceleration * dt;
    if (speed > maxSpeed) {
        speed = maxSpeed;
    }
}

void Vehicle::reverse(float dt) {
    speed -= acceleration * dt;
    if (speed < -maxSpeed) {
        speed = -maxSpeed;
    }
}

void Vehicle::deccelerate(float dt) {
    float lastSpeed = speed;
    if (speed > 0.0) {
        speed -= acceleration * deccelerationMult * dt;
    } else if (speed < 0.0) {
        speed += 2.0f * dt;
    }
    if (lastSpeed < 0.0 && speed > 0.0) {
        speed = 0.0;
    } else if (lastSpeed > 0.0 && speed < 0.0) {
        speed = 0.0;
    }

}

void Vehicle::brake(float dt) {
    deccelerate(dt*brakeMult);
}

void Vehicle::turn(float dt) {
    if (inputDirection.x > 0) {
        turnSpeed += turnAcceleration * dt;
        if (turnSpeed > maxTurnSpeed) {
            turnSpeed = maxTurnSpeed;
        }
    } else if (inputDirection.x < 0) {
        turnSpeed -= turnAcceleration * dt;
        if (turnSpeed < -maxTurnSpeed) {
            turnSpeed = -maxTurnSpeed;
        }
    }
}

void Vehicle::straighten(float dt) {
    if (turnSpeed > 0.0) {
        turnSpeed -= turnAcceleration * dt;
        if (turnSpeed < 0.0) {
            turnSpeed = 0.0;
        }
    } else if (turnSpeed < 0.0) {
        turnSpeed += turnAcceleration * dt;
        if (turnSpeed > 0.0) {
            turnSpeed = 0.0;
        }
    }

}

void Vehicle::Update(float dt) { // deltaTime
    inputDirection = glm::vec3(userInput.sideInput, 0,userInput.forwardInput);
    l_brakeLight.setColor(glm::vec3(0,0,0));
    if (inputDirection.z > 0) {
        accelerate(dt);
    } else if (inputDirection.z < 0 && speed > 0.0) {
        brake(dt);
        l_brakeLight.setColor(glm::vec3(1,0,0));
    } else if (inputDirection.z < 0 && speed <= 0.0) {
        reverse(dt);
        l_brakeLight.setColor(glm::vec3(1,0,0));
    } else {
        deccelerate(dt);
    }
    if (inputDirection.x != 0) {
        turn(dt);
    } else {
        straighten(dt);
    }
    rotation.y += glm::radians(turnSpeed) * std::clamp(speed/speedTurnThreshold,-1.0f,1.0f) * dt;

    direction = rotationToDirection(rotation);
    velocity = direction * speed * dt;
    body.setPosition(body.getPosition() + velocity);
    body.setOrientation(-rotation);

    frontLookat = body.getPosition() + direction * 10.0f;
    rearCamera = rearCamera + (body.getPosition() - direction * 10.0f + glm::vec3(0,3,0) - rearCamera) * std::min(dt/0.2f,1.0f);

    headlightPos = body.getPosition() + direction * 2.0f + glm::vec3(0,1,0);
    UpdateLights();
}

void Vehicle::UpdateLights() {
    headlights.setPosition(body.getPosition() + direction * 2.0f + glm::vec3(0,1,0));
    headlights.setDirection(glm::normalize(direction - glm::vec3(0,0.3,0)));

    l_brakeLight.setPosition(body.getPosition() - direction * 2.5f - glm::vec3(0,0,0));
    //l_brakeLight.setColor(glm::vec3())
}