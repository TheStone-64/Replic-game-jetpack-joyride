#pragma once
#include "background.hpp"
#include "player.hpp"
#include "managerLayer.hpp"
#include "hud.hpp"

const std::string TRACK = "../assets/sound/music.mp3";

class Game {
    private:
        Hud hud;
        Background bg;
        ManagerLayer manager;
        Player pl;

        inline static sf::Music backgroundMusic;

        float distanceAccumulator = 0.f;

    public:
        Game();
        ~Game() = default;
        
        int HUD_calcMetersToAdd(sf::Time dt, const sf::View &view, float scrollVelocity);
        bool update(sf::Time dt, const sf::View &view, float scrollVelocity);
        void draw(sf::RenderWindow& window);

        void playMusic();
        int getDistance(); 
        int getCoin();
        std::pair<int, int> getHudInfo() const ;

        void mousePressed(const sf::Event::MouseButtonPressed& event);
        void mouseRelease(const sf::Event::MouseButtonReleased& event);
        void keyPressed(const sf::Event::KeyPressed& event);
        void KeyReleased(const sf::Event::KeyReleased& event);
        bool isDead();
    
};