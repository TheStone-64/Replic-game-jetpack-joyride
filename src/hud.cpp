#include "hud.hpp"

Hud::Hud() :coinText(font), distanceText(font){
    if (!font.openFromFile(PATH_FONT)) {
        Debug::log(Debug::GAME, Debug::ERR, "Impossile load font for HUD.");
    }

    coinText.setFont(font);
    coinText.setCharacterSize(Config::HUD_CHARACTER_SIZE);
    coinText.setFillColor(Config::HUD_COIN_COLOR);
    coinText.setPosition(Config::HUD_FIST_LINE); 

    distanceText.setFont(font);
    distanceText.setCharacterSize(Config::HUD_CHARACTER_SIZE);
    distanceText.setFillColor(Config::HUD_METERS_COLOR);
    distanceText.setPosition(Config::HUD_SECOND_LINE); 
}

int Hud::getCoin () const {
    return numberCoin;
}

int Hud::getDistance() const {
    return numberDistance;
}


void Hud::addCoin(int number) { 
    numberCoin += number; 
}

void Hud::addDistance(int number) { 
    numberDistance += number; 
}


void Hud::setDistance(int number) {
    numberDistance = number;
}

void Hud::update(sf::Time time, const sf::View& view) {
    coinText.    setString("Coins "    + std::to_string(numberCoin));
    distanceText.setString("Distance " + std::to_string(numberDistance) + " m");
}

void Hud::draw(sf::RenderWindow& window) {
    sf::View currentView = window.getView();

    window.setView(window.getDefaultView());

    window.draw(coinText);
    window.draw(distanceText);

    window.setView(currentView);
}