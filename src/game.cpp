#include "game.hpp"
#include <utility>

using namespace Debug;

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

Game::Game() {
    log(GAME, INFO, "Loading game...");

    if (!backgroundMusic.openFromFile(TRACK)) { 
        log(GAME, ERR, "Impossible to load sound of game");
    }

    //backgroundMusic.play();
    backgroundMusic.setVolume(Config::VOLUME_MUSIC);
    backgroundMusic.setLooping(true);

    // Configurazione sezioni statiche
    SingleCoin::loadTextures();
    CoinPattern::loadSound();
    
    Laser::loadTextures();
    Laser::loadSound();

    LaserBeam::loadTextures();
    LaserBeamHandler::loadSound();

    log(GAME, INFO, "... Game loaded");
}


// #########################################################################################################################################################################
// Uso variabile float distanceAccumulator per tener traccia della "distanza" percorso, quando questa distanza supera la variabile PIXEL_FOR_METERS 
// Vado a calcolare un intero ( in quando sulla HUD ci va un intero ) per poi sottrarlo a distance non eseguo distance - distance in quanto meters è un intero ed è diverso 
//      da un float e se svolgessi distance - distance perderei dei pixel 
int Game::HUD_calcMetersToAdd(sf::Time time, const sf::View &view, float scrollVelocity){    
    distanceAccumulator += scrollVelocity;
    int metersToAdd = 0;

    if (distanceAccumulator >= Config::PIXEL_FOR_METER) {
        metersToAdd = static_cast<int>(distanceAccumulator / Config::PIXEL_FOR_METER);
        distanceAccumulator -= metersToAdd * Config::PIXEL_FOR_METER;
    }

    //
    hud.update(time, view);
    return metersToAdd;
}

// #########################################################################################################################################################################
bool Game::update(sf::Time time, const sf::View &view, float scrollVelocity){
    
    bg.update(view);
    pl.update(time, scrollVelocity);
    std::pair<int, bool> controlHit = manager.update(time, view, pl.getBounds(), scrollVelocity);

    if (controlHit.second == true) {
        pl.kill(); 
    }

    hud.addCoin(controlHit.first);
    hud.addDistance(HUD_calcMetersToAdd(time, view, scrollVelocity));

    return pl.isDead();
}

void Game::draw(sf::RenderWindow& window){
    bg.draw(window);
    pl.draw(window);
    manager.draw(window);
    hud.draw(window);
}

void Game::playMusic(){
    backgroundMusic.play();
}

// #########################################################################################################################################################################

int Game::getDistance() {
    return hud.getDistance();
}

int Game::getCoin() {
    return hud.getCoin();
}
        

std::pair<int, int> Game::getHudInfo() const {
    return std::pair<int, int> (hud.getCoin(), hud.getDistance());
}

void Game::mousePressed(const sf::Event::MouseButtonPressed& event) {
    if (event.button == sf::Mouse::Button::Left) {
        pl.handleSpace_press();
    }
}

void Game::mouseRelease(const sf::Event::MouseButtonReleased& event) {
    if (event.button == sf::Mouse::Button::Left) {
        pl.handleSpace_release();
    }
}

void Game::keyPressed(const sf::Event::KeyPressed& event) {
    if (event.scancode == sf::Keyboard::Scancode::Space) {
        pl.handleSpace_press();
    }
}


void Game::KeyReleased(const sf::Event::KeyReleased& event) {
    if (event.scancode == sf::Keyboard::Scancode::Space) {
        pl.handleSpace_release();
    }
}

bool Game::isDead(){
    return pl.isDead();
}