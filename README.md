# Desert Drive OpenGL

A C++/OpenGL graphics demo initially created for the Computer Graphics course at CSU Long Beach.  
Use the WASD controls to drive a Jeep through a desert landscape!

This project demonstrates:
- Real-time lighting and shading
- Texture mapping
- Animation

---

## Platform Support

This project is developed and tested on **macOS**.  
It should also build on **Linux** and **Windows** with CMake, assuming equivalent dependencies are installed.

---

## Dependencies

These dependencies are required to build the project on any supported OS:

- **C++17 compiler** — e.g. `g++`, `clang++`, or MSVC
- **CMake ≥ 3.x** — build system generator
- **SFML** — window/context management, input
- **GLM** — mathematics for graphics
- **Assimp** — model import
- **stb_image** — image loading (included in repo)
- **OpenGL** — (loaded via glad)
- **glad** — OpenGL loader (included in repo)

---

## Quick Start (macOS)

### Prerequisites

| Tool/Library | Purpose                  | Website                                                         |
|--------------|--------------------------|-----------------------------------------------------------------|
| Xcode CLT    | Compiler/Build tools      | [developer.apple.com/xcode](https://developer.apple.com/xcode/) |
| Homebrew     | Package manager           | [brew.sh](https://brew.sh/)                                     |
| CMake        | Build system generator    | [cmake.org](https://cmake.org/)                                 |
| SFML         | Graphics library          | [sfml-dev.org](https://www.sfml-dev.org/)                       |
| GLM          | Math library              | [glm.g-truc.net](https://glm.g-truc.net/)                       |
| Assimp       | Model import library      | [assimp.org](https://www.assimp.org/)                           |

### Build & Run

```sh
# Install Xcode Command Line Tools
xcode-select --install

# Install Homebrew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install CMake and required libraries
brew install cmake sfml glm assimp

# Clone and build the project
git clone https://github.com/mleedev/desert-drive-opengl.git
cd desert-drive-opengl
cmake -B build
cmake --build build

# Run the application
./build/mattsquared_graphics
```

---

## Running the App

Upon running the executable, a window will open displaying a 3D desert environment with a controllable Jeep.  
Use the listed controls to drive through the terrain and toggle camera views or lighting.

---

## Controls

- **W**: Accelerate
- **S**: Brake/Reverse
- **A**: Turn Left
- **D**: Turn Right
- **F**: Toggle High Beams
- **V**: Toggle Camera View

---

## Libraries Used

- **OpenGL**: Core graphics API, accessed via [glad](https://github.com/Dav1dde/glad) (included).
- **GLM**: Header-only C++ mathematics library for graphics.
- **SFML**: Simple and Fast Multimedia Library for window management and input.
- **Assimp**: Library for importing 3D model formats.
- **stb_image**: Header-only image loader (wrapped in a C++ class `StbImage.h`/`StbImage.cpp`).
- **khrplatform.h**: Platform abstraction header from Khronos (included via glad).

See [Credits](#credits) for third-party asset attributions.

---

## Possible Future Improvements

- Windows and Linux build instructions and support
- Enhanced lighting (e.g., dynamic shadows)
- Sound effects integration

---

## Credits

**Models:**
- [MILITARY JEEP](https://www.cgtrader.com/products/military-jeep-9fe110bf-0316-47d0-b84e-2ef51aeca732)
- [Desert PS1 FREE DEMO](https://www.cgtrader.com/products/desert-ps1-free-demo)

**Developed by:**
- Matthew Lee ([mleedev](https://github.com/mleedev))
- Matthew Halderman ([Immersor-git](https://github.com/Immersor-git))

**Course Information:**
- University: California State University, Long Beach
- Course: Computer Graphics - CECS 449, Spring 2024
- Instructor: Neal Terrell