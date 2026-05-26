#include "laser.hpp"

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################
// Funzioni di base

void Laser::playSound(){
    laserSound.play();
}

sf::FloatRect Laser::getBounds() const { 
    return sprite.getGlobalBounds(); 
}

sf::Vector2f Laser::getLaserSize(){
    return getBounds().size;
}

void Laser::setLaserPosition(sf::Vector2f pos) {
    sprite.setPosition(pos);
}

bool Laser::checkCollision(const sf::FloatRect& playerBounds)  {
    sf::FloatRect bounds = sprite.getGlobalBounds();
    
    if (type == LaserType::DIAGONAL) {
        
        // Calcola 5 sotto rettangoli sulla diagonale dell'oggetto 
        float stepX = bounds.size.x / 5.f;
        float stepY = bounds.size.y / 5.f;
        
        sf::FloatRect box1({bounds.position.x + stepX * 4, bounds.position.y}, {stepX, stepY});        
        sf::FloatRect box2({bounds.position.x + stepX * 3, bounds.position.y + stepY}, {stepX, stepY});
        sf::FloatRect box3({bounds.position.x + stepX * 2, bounds.position.y + stepY * 2}, {stepX, stepY});
        sf::FloatRect box4({bounds.position.x + stepX * 1, bounds.position.y + stepY * 3}, {stepX, stepY});
        sf::FloatRect box5({bounds.position.x,             bounds.position.y + stepY * 4}, {stepX, stepY});

        if( playerBounds.findIntersection(box1).has_value() || playerBounds.findIntersection(box2).has_value() ||
            playerBounds.findIntersection(box3).has_value() || playerBounds.findIntersection(box4).has_value() ||
            playerBounds.findIntersection(box5).has_value()) 
        {
            playSound();
            return true;
        }

        return false;
    } 

    // Caso laser orizzontale / verticale 
    float centerX = bounds.getCenter().x;
    float centerY = bounds.getCenter().y;

    float newWidth  = bounds.size.x * ((type == LaserType::VERTICAL)  ? 0.1f : 0.8f);          
    float newHeight = bounds.size.y * ((type == LaserType::HORIZONTAL)? 0.1f : 0.8f);      

    float newLeft = centerX - (newWidth / 2.f);
    float newTop  = centerY - (newHeight / 2.f);

    sf::FloatRect lethalBox({newLeft, newTop}, {newWidth, newHeight});

    if(playerBounds.findIntersection(lethalBox).has_value()){
        playSound();
        return true;
    }
    
    return false;
}

void Laser::update(sf::Time dt) {}

void Laser::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

// #########################################################################################################################################################################
// #########################################################################################################################################################################
// #########################################################################################################################################################################

bool LaserHandler::shouldCreate(const sf::View& view, float incrementDistance) {
    return canSpawn(view, incrementDistance, activeLaser.size());
}


//--->
LaserType handlerGen(){
    return static_cast<LaserType>(
        std::uniform_int_distribution<int>(0, Config::LASER_NUMBER_MODELS - 1)(generator)
    );
}

LaserType LaserHandler::generateLaserType() {
    LaserType newType = handlerGen();
    
    while (newType == lastGen)
        newType = handlerGen();
    
    lastGen = newType;
    return newType;
}
//<---

void LaserHandler::addLaser(LaserType choice, sf::Vector2f spawnPos){
    activeLaser.push_back(std::make_unique<Laser>(choice, spawnPos));
}    

bool LaserHandler::checkCollisions(const sf::FloatRect& playerBounds){
    for(auto& laser : activeLaser){
        if(laser->checkCollision(playerBounds)){
            laser->playSound();
            return true;
        }
    }

    return false;
}

void LaserHandler::update(sf::Time dt, const sf::View& view){
    for(auto& laser : activeLaser) {
        laser->update(dt);
    }

    activeLaser.erase(
        std::remove_if(
                activeLaser.begin(),
                activeLaser.end(), 
                [&view](const auto& activeLaser) 
                {
                    return (activeLaser->getPosition().x + Config::LASER_OFFSET < view.getCenter().x - (view.getSize().x / 2.f));
                }
            ), 
            activeLaser.end()
    );
}

int LaserHandler::getNumberLaserActive() {
    return activeLaser.size();
}

bool LaserHandler::areThereActiveLaser(){
    return getNumberLaserActive() > 0;
}   

void LaserHandler::draw(sf::RenderWindow& window) {
    for (auto& laser : activeLaser) {
        laser->draw(window);
    }
}

// #########################################################################################################################################################################
// Funzione "speciale" resituisce un vector con tutte le aree dei laser
std::vector<sf::FloatRect> LaserHandler::getOccupiedAreas() const {
    std::vector<sf::FloatRect> areas;
    areas.reserve(activeLaser.size()); 
    
    for (const auto& laser : activeLaser) {
        areas.push_back(laser->getBounds());
    }    
    
    return areas;
}    
