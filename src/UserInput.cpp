//
// Created by Matthew Halderman on 5/9/24.
// Refactored by Matthew Lee on 7/9/25.
//
#include <SFML/Window/Event.hpp>
#include "../include/UserInput.h"
void UserInput::processInput(const sf::Event::KeyPressed& e) {
    if (e.scancode == sf::Keyboard::Scancode::W) {
        // Forward
        wPressed = true;
        forwardInput = 1;
    } else if (e.scancode == sf::Keyboard::Scancode::S) {
        // Backward
        sPressed = true;
        forwardInput = -1;
    } else if (e.scancode == sf::Keyboard::Scancode::A) {
        // Left
        aPressed = true;
        sideInput = -1;
    } else if (e.scancode == sf::Keyboard::Scancode::D) {
        // Right
        dPressed = true;
        sideInput = 1;
    } else if (e.scancode == sf::Keyboard::Scancode::F) {
        // Toggle high beams
        highBeams = !highBeams;
    } else if (e.scancode == sf::Keyboard::Scancode::V) {
        // Toggle camera view
        cameraView = (cameraView + 1) % 5;
    } else if (e.scancode == sf::Keyboard::Scancode::N) {
        // timeOfDay = (timeOfDay + 1) % 2;
    } else {
        // Handle other keys if necessary
        return;
    }
}

void UserInput::processInput(const sf::Event::KeyReleased& e) {
    if (e.scancode == sf::Keyboard::Scancode::W) {
        // Forward
        wPressed = false;
        forwardInput = sPressed ? -1 : 0;
    } else if (e.scancode == sf::Keyboard::Scancode::S) {
        // Backward
        sPressed = false;
        forwardInput = wPressed ? 1 : 0;
    } else if (e.scancode == sf::Keyboard::Scancode::A) {
        //left
        aPressed = false;
        sideInput = dPressed ? 1 : 0;
    } else if (e.scancode == sf::Keyboard::Scancode::D) {
        // Right
        dPressed = false;
        sideInput = aPressed ? -1 : 0;
    }
}
