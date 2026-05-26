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

### 3. Execution
cd build

./game

## 🎨 Credits & Resources

### Assets
All in-game images and sound effects were sourced from a public GitHub repository, with the exception of the main menu background, which was generated using Google Gemini.
[Link to the repository assets](https://github.com/rsharifnasab/sbu_JetPackJoyRide/tree/master/assets/pic)

### Fonts
Both fonts used in this project are public domain and freely available for commercial use.
[1001 Fonts - Video Game Fonts](https://www.1001fonts.com/video-game-fonts.html)

# Gameplay

<img width="1291" height="490" alt="menu" src="https://github.com/user-attachments/assets/4e11eb05-f003-48fa-8773-4f0c2df92731" />
<img width="1291" height="490" alt="gameplay1" src="https://github.com/user-attachments/assets/cf19d538-7972-4be3-a91d-7c196173f744" />
<img width="1291" height="490" alt="gameplay 2 1" src="https://github.com/user-attachments/assets/fa3b9a4e-b119-41b1-bdd0-a69f97ff4d15" />
<img width="1291" height="490" alt="gameplay2 2" src="https://github.com/user-attachments/assets/f911169a-1d62-4065-ad1c-55a15d2949b5" />
