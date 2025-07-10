# Desert Drive OpenGL

A C++/OpenGL graphics demo initially created for the Computer Graphics course at CSU Long Beach.
Use the WASD controls to drive a Jeep through a desert landscape!

This project demonstrates various graphics rendering techniques including:
- Real-time lighting and shading
- Texture mapping
- Animation

## Controls:
- **W**: Accelerate
- **S**: Brake/Reverse
- **A**: Turn Left
- **D**: Turn Right
- **F**: Toggle High Beams
- **V**: Toggle Camera View

## Quick Start (macOS):

### Prerequisites

- [Xcode Command Line Tools](https://developer.apple.com/xcode/)
- [Homebrew](https://brew.sh/)
- [CMake](https://cmake.org/)

### Install Dependencies

Open Terminal and run:
```sh
brew install cmake sfml glm assimp
```

### Build and Run

Clone the repository and navigate to the project directory:

```sh
git clone https://github.com/mleedev/desert-drive-opengl.git
cd desert-drive-opengl
cmake -B build
cmake --build build
./build/mattsquared_graphics
```

## Credits:
Models: 
- [MILITARY JEEP](https://www.cgtrader.com/products/military-jeep-9fe110bf-0316-47d0-b84e-2ef51aeca732)
- [Desert PS1 FREE DEMO](https://www.cgtrader.com/products/desert-ps1-free-demo)

Developed by:
- Matthew Lee (GitHub: [mleedev](https://github.com/mleedev))
- Matthew Halderman (GitHub: [Immersor-git](https://github.com/Immersor-git))

Course Information:
- University: California State University, Long Beach
- Course Title: Computer Graphics - CECS 449, Spring 2024
- Instructor: Neal Terrell