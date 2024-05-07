/**
This application renders a textured mesh that was loaded with Assimp.
*/

#include <glad/glad.h>
#include <iostream>

#include "Mesh3D.h"
#include "ShaderProgram.h"
#include "Scene.h"

int main() {
	// Initialize the window and OpenGL.
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
    Settings.majorVersion = 4;
    Settings.minorVersion = 1;
    Settings.attributeFlags = sf::ContextSettings::Attribute::Core;
	sf::Window window(sf::VideoMode{ 1200, 800 }, "SFML Demo", sf::Style::Resize | sf::Style::Close, Settings);
	gladLoadGL();
	glEnable(GL_DEPTH_TEST);

	// Initialize scene objects.
	auto scene = Scene::jeep();

	auto cameraPosition = glm::vec3(0,2,5);//glm::vec3(0, 0, 5);
	auto camera = glm::lookAt(cameraPosition, glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	auto perspective = glm::perspective(glm::radians(45.0), static_cast<double>(window.getSize().x) / window.getSize().y, 0.1, 100.0);

	ShaderProgram& mainShader = scene.defaultShader;
	mainShader.activate();
	mainShader.setUniform("view", camera);
    mainShader.setUniform("projection", perspective);
    mainShader.setUniform("ambientColor",glm::vec3(0.3,0.3,0.3));
	// Ready, set, go!
	for (auto& animator : scene.animators) {
		animator.start();
	}
    int uIndex = 0;
    for (auto& light : scene.lights) {
        light.setUniformIndex(uIndex++);
        light.updateUniforms(mainShader);
    }
	bool running = true;
	sf::Clock c;
    float counter = 0.0f;

	auto last = c.getElapsedTime();
	while (running) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				running = false;
			}
		}
		
		auto now = c.getElapsedTime();
		auto diff = now - last;
		auto diffSeconds = diff.asSeconds();
		last = now;
		for (auto& animator : scene.animators) {
			//animator.tick(diffSeconds);
		}
        //Some test code to move a spotlight around
        glm::vec3 lightPos = glm::vec3(sin(counter)*4,0+sin(counter*0.1234)*0.3,cos(counter)*4);
        scene.lights[2].setPosition(lightPos); //Moves the spotlight around
        scene.lights[2].setDirection(-glm::normalize(lightPos));
        scene.lights[2].updateUniforms(mainShader); //Call this whenever you change the light's properties
        scene.objects[1].setPosition(lightPos); //Moves the tiger model to the light position
        counter += diff.asSeconds();
		// Clear the OpenGL "context".
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Render each object in the scene.
		for (auto& o : scene.objects) {
			o.render(window, mainShader);
		}
		window.display();
	}

	return 0;
}


