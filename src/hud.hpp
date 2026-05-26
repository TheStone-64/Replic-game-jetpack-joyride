#pragma once
#include "Global.hpp"

inline const std::string PATH_FONT = "../assets/fonts/PIXY.ttf";

class Hud {
    private:
        int numberCoin = 0;
        int numberDistance = 0;

        sf::Font font;
        sf::Text coinText;
        sf::Text distanceText;

    public:
        Hud();

        int getCoin () const;
        int getDistance() const;
        void addCoin(int number);
        void addDistance(int number);

        void setDistance(int number);
        void update(sf::Time time, const sf::View& view);
        void draw(sf::RenderWindow& window);
};