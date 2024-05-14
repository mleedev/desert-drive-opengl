//
// Created by Matthew Halderman on 5/9/24.
//

#include <iostream>
#include "Vehicle.h"
#include "Object3D.h"

const float maxTurnSpeed = 80.0f;
const float turnAcceleration = 320.0f;
const float maxSpeed = 40.0f;
const float acceleration = 12.0f;
const float deccelerationMult = 0.5f;
const float brakeMult = 4.0f;
const float speedTurnThreshold = 15.0f;

void Vehicle::SetLights() {
    headlights.setLightSpaceMatrix(glm::mat4(
            35,0,0,0, //Position
            1,1,0.7,0, //Color
            3,50,55,0, //LightType, Range, Cuttoff Angle (For spotlights)
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

float getGearMult(float sp) {
    float perc = sp/maxSpeed;
    if (perc < 0.05) {
        return 2.0f;
    } else if (perc < 0.25) {
        return 1.5f;
    } else if (perc < 0.5) {
        return 1.0f;
    } else if (perc < 0.75) {
        return 0.5f;
    } else {
        return 0.25f;
    }
}

void Vehicle::accelerate(float dt) {
    float gearMult = getGearMult(speed);
    accelPower = gearMult;
    speed += acceleration * gearMult * dt;
    if (speed > maxSpeed) {
        speed = maxSpeed;
    }
}

void Vehicle::reverse(float dt) {
    float gearMult = getGearMult(speed);
    accelPower = -gearMult;
    speed -= acceleration * gearMult * dt;
    if (speed < -maxSpeed) {
        speed = -maxSpeed;
    }
}

void Vehicle::deccelerate(float dt) {
    float lastSpeed = speed;;
    if (speed > 0.0) {
        speed -= acceleration * deccelerationMult * dt;
        accelPower = -deccelerationMult;
    } else if (speed < 0.0) {
        speed += acceleration * deccelerationMult * dt;
        accelPower = deccelerationMult;
    }
    if (lastSpeed < 0.0 && speed > 0.0) {
        speed = 0.0;
    } else if (lastSpeed > 0.0 && speed < 0.0) {
        speed = 0.0;
    }
}

void Vehicle::brake(float dt) {
    deccelerate(dt*brakeMult);
    accelPower = speed < 0.0 ? brakeMult : -brakeMult;
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

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Vehicle::Update(float dt) { // deltaTime
    inputDirection = glm::vec3(userInput.sideInput, 0,userInput.forwardInput);
    l_brakeLight.setColor(glm::vec3(0,0,0));
    accelPower = 0.0f;
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
    turnPower = turnSpeed/maxTurnSpeed * speed/maxSpeed;
    rotation.y -= (glm::radians(turnSpeed) * std::clamp(speed/speedTurnThreshold,-1.0f,1.0f) * dt);
    if (rotation.y < -glm::pi<float>()) {
        rotation.y += 2*glm::pi<float>();
    } else if (rotation.y > glm::pi<float>()) {
        rotation.y -= 2*glm::pi<float>();
    }
    accelTilt = accelTilt + (accelPower - accelTilt) * std::min(dt*(2.0f),1.0f);
    turnTilt = turnTilt + (turnPower - turnTilt) * std::min(dt*(4.0f),1.0f);
    float xmultX = sin(rotation.y);
    float zmultX = cos(rotation.y);
    float xmultZ = sin(rotation.y-glm::radians(90.0f));
    float zmultZ = cos(rotation.y-glm::radians(90.0f));
    rotation.x = accelTilt * glm::radians(5.0f) * xmultX + turnTilt * glm::radians(7.0f) * xmultZ;
    rotation.z = accelTilt * glm::radians(5.0f) * zmultX + turnTilt * glm::radians(7.0f) * zmultZ;

    direction = rotationToDirection(glm::vec3(0,-rotation.y,0));
    velocity = direction * speed * dt;
    body.setPosition(body.getPosition() + velocity);
    body.setOrientation(rotation);
    auto& leftTire = body.getChild(0);
    leftTire.rotate(glm::vec3(0,speed/maxSpeed*0.5f,0));

    frontLookat = body.getPosition() + direction * 10.0f;

    glm::vec3 rearTarget;
    float followSpeedTarget = 0;
    if (userInput.cameraView == 0) {
        rearTarget = body.getPosition() - direction * 10.0f + glm::vec3(0,3,0);
        followSpeedTarget = 5.0f;
    } else if (userInput.cameraView == 1) {
        rearTarget = body.getPosition() + direction * 2.0f + glm::vec3(0,1.5,0);
        followSpeedTarget = 100.0f;
    } else if (userInput.cameraView == 2) {
        rearTarget = body.getPosition() - direction * 20.0f + glm::vec3(0,20,0);
        followSpeedTarget = 10.0f;
    } else if (userInput.cameraView == 3) {
        rearTarget = glm::vec3(0,0,0);//body.getPosition() + direction * 50.0f + glm::vec3(0,0,0);
        followSpeedTarget = 5.0f;
    }

    followSpeed = followSpeed + (followSpeedTarget - followSpeed) * std::min(dt*(5.0f),1.0f);

    rearCamera = rearCamera + (rearTarget - rearCamera) * std::min(dt*(followSpeed),1.0f);

    headlightPos = body.getPosition() + direction * 2.0f + glm::vec3(0,1,0);
    UpdateLights();
}

void Vehicle::UpdateLights() {
    float heightAdd = 1.0f;
    if (userInput.highBeams) {
        heightAdd = 1.2f;
        headlights.setRange(80.0f);
        headlights.setCutoffAngle(70.0f);
        headlights.setColor(glm::vec3(1,1,1.2));
    } else {
        headlights.setRange(35.0f);
        headlights.setCutoffAngle(45.0);
        headlights.setColor(glm::vec3(1,1,0.7));
    }
    headlights.setPosition(body.getPosition() + direction * 2.0f + glm::vec3(0,heightAdd,0));
    headlights.setDirection(glm::normalize(direction - glm::vec3(0,0.3,0)));

    l_brakeLight.setPosition(body.getPosition() - direction * 2.25f + glm::vec3(0,0.9,0));
    //l_brakeLight.setColor(glm::vec3())
}