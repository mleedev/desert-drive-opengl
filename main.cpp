/**
This application displays a mesh in wireframe using "Modern" OpenGL 3.0+.
The Mesh3D class now initializes a "vertex array" on the GPU to store the vertices
	and faces of the mesh. To render, the Mesh3D object simply triggers the GPU to draw
	the stored mesh data.
We now use explicit vertex and fragment shaders instead of the default OpenGL implementations.
	See "no_transform.vert" for a vertex shader that sets a vertex's clip-space coordinates
		equal to its local-space coordinates.
	See "all_green.frag" for a fragment shader that sets a pixel to green, always.
*/

#include <iostream>
#include <memory>
#include <glad/glad.h>

#include "Mesh3D.h"
#include "Object3D.h"
#include "AssimpImport.h"
// You will need to add your own AssimpImport.cpp from HW 4 if you want to load
// other meshes.

#include "ShaderProgram.h"
#include "StbImage.h"

int main() {
	// Initialize the window and OpenGL.
	sf::ContextSettings Settings;
	Settings.depthBits = 24; // Request a 24 bits depth buffer
	Settings.stencilBits = 8;  // Request a 8 bits stencil buffer
	Settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
    Settings.majorVersion = 4;
    Settings.minorVersion = 1;
	sf::Window window(sf::VideoMode{ 1000, 1000 }, "SFML Demo", sf::Style::Resize | sf::Style::Close, Settings);
	gladLoadGL();

	// Load shaders and bind them for use.
	ShaderProgram defaultShader;
	try {
		defaultShader.load(
                "../shaders/textured_perspective.vert",
                "../shaders/texture_mapped.frag");
	}
	catch (std::runtime_error& e) {
		std::cout << "ERROR: " << e.what() << std::endl;
		return 1;
	}

	// Inintialize scene objects.

	defaultShader.activate();
	glm::mat4 camera = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	glm::mat4 perspective = glm::perspective(glm::radians(45.0), static_cast<double>(window.getSize().x) / window.getSize().y, 0.1, 100.0);
	defaultShader.setUniform("view", camera);
	defaultShader.setUniform("projection", perspective);

	/*
	auto bunny = assimpLoad("models/bunny_textured.obj", true);
	bunny.move(glm::vec3(0.2, -1, -5));
	bunny.grow(glm::vec3(9, 9, 9));
	*/

	/*
	This draws the example from lecture: a triangle with a brick wall texture.
	*/
	StbImage texture;
	texture.loadFromFile("../models/wall.jpg");
	auto obj = Object3D(std::make_shared<Mesh3D>(Mesh3D::triangle(texture)));
	obj.move(glm::vec3(0, 0, -3));
	
	// Ready, set, go!
	bool running = true;
	sf::Clock c;

	auto last = c.getElapsedTime();
	while (running) {
		sf::Event ev;
		while (window.pollEvent(ev)) {
			if (ev.type == sf::Event::Closed) {
				running = false;
			}
		}

		// Clear the OpenGL "context".
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		obj.render(window, defaultShader);
		window.display();


		auto now = c.getElapsedTime();
		auto diff = now - last;
		std::cout << 1 / diff.asSeconds() << " FPS " << std::endl;
		last = now;
	}

	return 0;
}


