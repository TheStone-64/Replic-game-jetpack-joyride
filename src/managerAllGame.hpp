#pragma once
#include "Global.hpp"
#include "game.hpp"
#include "menu.hpp"

enum class State {
    GAME, 
    MENU
};

class ManagerAllGame {

    private:
        State state;
        
        sf::RenderWindow window;
        sf::View view;
        sf::Clock clock;
        
        float currentSpeed;
        float speed;
        bool waitActionAfterDead;

        std::unique_ptr<Menu> menu;
        std::unique_ptr<Game> game; 
        std::pair<int, int> traceCoinScore; 
        void reload();
        
        void processEvents();
        void update(sf::Time time);
        void render();

    public:
        ManagerAllGame();
        void run(); 
};