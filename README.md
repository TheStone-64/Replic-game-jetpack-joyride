# Jetpack Joyride Clone (C++ / SFML 3)
Welcome to my Jetpack Joyride clone! This project is a 2D endless runner game inspired by the classic mobile hit, developed entirely from scratch in C++ using the modern SFML 3 multimedia library.

## 🚀 Key Features
Endless Scrolling & Physics: Smooth infinite background scrolling and gravity/thrust-based jetpack mechanics.

Procedural Generation: Dynamic, randomized spawning of obstacles (horizontal, vertical, and diagonal static zappers) and collectible coins arranged in geometric patterns (squares, snakes, etc.).

Advanced Boss-like Obstacles: Timed, screen-wide "Laser Beams" complete with warning phases, state machines, and custom screen-adaptive rendering.

Precise Collision Detection: Custom AABB (Axis-Aligned Bounding Box) hitboxes mathematically tailored for diagonal obstacles to ensure fair and accurate gameplay.

State Management: Finite State Machines (FSM) controlling player status (Alive, Dying, Dead), dynamic laser mechanics, and overall game flow (Menu, Loading, Gameplay).

Audio & Visuals: Full integration of SFML Audio for background music and asynchronous sound effects (Jetpack thrust, coin collection, zapper shocks), alongside animated sprites.

Optimized Memory Management: Efficient resource allocation using smart pointers (std::unique_ptr) and lazy deletion (erase-remove idiom) to prevent memory leaks and maintain a steady 60+ FPS frame rate.

## 🛠️ Build Instructions
The project is configured with CMake to ensure cross-platform compilation (Windows, Linux, macOS).

Bash
### 1. Configure the project and generate the build folder
cmake -B build -D CMAKE_BUILD_TYPE=Release

### 2. Compile the executables
cmake --build build

cd build

./game
