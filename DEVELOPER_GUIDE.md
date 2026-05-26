# Custom Debug & Logging System (Global.hpp)

To avoid a cluttered console, the project uses a custom, highly modular logging system built with C++ variadic templates (`print_args`).

The logger is defined in the `Debug` namespace and filters messages based on two constraints:

*   **Modules**: `BACKGROUND`, `PLAYER`, `GAME`, `COIN`, `LASER`, `MENU`, etc.
*   **Levels**: `INFO`, `DBG`, `WARN`, `ERR`.

## How it works:

Inside `Global.hpp`, you can toggle specific modules or levels by modifying the `active_modules` and `active_levels` sets. If `all_modules` or `all_levels` is set to `true`, the system bypasses the filters.

```cpp
// Example of usage in code:
Debug::log(Debug::PLAYER, Debug::INFO, "Player texture loaded successfully.");
```

## 2. Core Architecture & State Management

The application is governed by a strict hierarchy to ensure the Separation of Concerns:

*   **ManagerAllGame**: The highest-level controller. It runs the main window loop and acts as a Finite State Machine (FSM), switching the application state between `State::MENU` and `State::GAME`. It intercepts user input and routes it to the active state.
*   **Game**: The core gameplay engine. It encapsulates the Player, Background, Hud, and ManagerLayer. It handles the conversion of pixel progression into "meters" and passes the scrolling speed (`Config::SCROLL_SPEED * DeltaTime`) to all entities.

## 3. Spatial Management & Anti-Overlap (ManagerLayer.cpp)

One of the most complex challenges in procedural generation is preventing objects (like coins and lasers) from spawning on top of each other. This is handled by the `ManagerLayer`.

### The Safe-Spawning Algorithm:

1.  The manager holds a map tracking all active hitboxes: `std::map<EntityType, std::vector<sf::FloatRect>> areaTaken`.
2.  Before a new obstacle or coin pattern is spawned, the manager retrieves a fused list of all occupied spaces via `getAllOccupiedAreas()`.
3.  It generates candidate coordinates and tests them using `Utils::isAreaSafe()`, which adds a safety padding around existing objects.
4.  If the area is safe, the spawn callback (passed via a lambda function in `handlerGenericUpdate`) is executed. If it fails after a maximum number of attempts (`Config::MAX_TRY_SEARCH_AREA_FREE`), the spawn is safely aborted to prevent infinite loops.

## 4. Entity System & Memory Management

*   All in-game objects inherit from a base `Entity` interface (guaranteeing polymorphic `update` and `draw` methods).
*   Managers (like `CoinHandler` and `LaserHandler`) inherit from `BaseHandler` (or `EventInView_BaseHandler` for dynamic events like Laser Beams) to share the procedural generation logic.
*   Memory is strictly managed using Smart Pointers (`std::unique_ptr`). When objects fall out of the camera view, they are safely deallocated using the erase-remove idiom combined with lambda conditions, ensuring zero memory leaks during infinite scrolling.

## 5. Player Physics & Flight Mechanics (Player.cpp)

The `Player` class does not rely on a built-in physics engine; instead, it uses a custom `PhysicsComponent` struct to handle vertical kinematics.

*   **Input Handling**: Pressing the spacebar triggers `handleSpace_press()`, setting `physics.isFlying = true`.
*   **Gravity & Thrust**: Inside the `update()` loop, if the player is flying, a negative vertical thrust (`Config::THRUST`) is applied to `physics.velocity`. When the spacebar is released, gravity (`Config::GRAVITY`) takes over, pulling the player down.
*   **Boundaries**: The logic clamps the player's position to prevent flying above the screen (`Config::PLAYER_PADDING`) or falling below the floor level.
*   **State Machine**: The player operates on an FSM (ALIVE, DYING, DEAD). A lethal collision triggers the DYING state, disabling input, applying a knockback impulse, and forcing a rapid descent until the player hits the floor and transitions to DEAD.
