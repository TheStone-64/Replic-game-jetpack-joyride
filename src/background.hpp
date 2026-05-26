#pragma once
#include "Global.hpp"

const std::string BG_START_LAB = "../assets/pic/back/LabStart.png";
const std::string BG_LAB_FIRST = "../assets/pic/back/Lab1.png";
const std::string BG_LAB_SECOND = "../assets/pic/back/Lab2.png";

const int BG_ALL = 3;

class Background {
    private:
        inline static sf::Texture textures[BG_ALL];
        std::vector<sf::Sprite> sprites;
        float dimSprite_x[BG_ALL];
        float dimTotal = 0;
        
    public:
        Background();
        void update(const sf::View& view);
        void draw(sf::RenderWindow& window);
};