#include "managerLayer.hpp"

// Funzione generica per estrapolare dalla map tutte le aree
std::vector<sf::FloatRect> ManagerLayer::getAllOccupiedAreas() const{
    std::vector<sf::FloatRect> totalOccupied;

    size_t totalSize = 0;
    for (auto const& [type, rects] : areaTaken){
        totalSize += rects.size();
    }
        
    totalOccupied.reserve(totalSize); 

    for (auto const& [type, rects] : areaTaken) {
        totalOccupied.insert(totalOccupied.end(), rects.begin(), rects.end());
    }

    return totalOccupied;
}

// Partendo dalle singole funzioni di handler specializzate ho costruito un metodo generico per  ìl'update automatico usando delle sottofunzioni handler per 
//  la preparzione dei parametri ( l'ultimo parametro è una funzione lambda );
template <typename EntityType, typename SpawnCallback>
bool ManagerLayer::handlerGenericUpdate (
    std::unique_ptr<EntityType>& obj, 
    sf::Vector2f targetSize, 
    const std::vector<sf::FloatRect>& allOccupied, 
    const sf::View& view, 
    float speed, 
    int maxTry, 
    SpawnCallback onSpawn 
) {
    if(!obj->shouldCreate(view, speed)) return false;

    float viewRight = view.getCenter().x + (view.getSize().x / 2.f);
    float xPos = obj->getNextSpawnX(viewRight); 

    for (int i = 0; i < maxTry; ++i) {
        float yPos = Utils::generateYPos(targetSize.y);
        sf::FloatRect candidateRect({xPos, yPos}, targetSize);

        if (Utils::isAreaSafe(candidateRect, allOccupied)) {
            sf::Vector2f finalPos = {xPos, yPos};
            
            onSpawn(finalPos); 

            obj->resetSpawnDistance(finalPos.x);
            return true;
        }   
    }
    
    return false;
}

// Funzione di preprazione dei parametri per il metodo generico
bool ManagerLayer::coin_handlerCallGenericUpdate(const sf::View& view, float speed, const std::vector<sf::FloatRect>& allOccupied) {
    
    CoinType coinType = coin->generateCoinType();
    sf::Vector2f patternSize = CoinPattern::getPatternSize(coinType);
    
    return handlerGenericUpdate(
        coin, patternSize, allOccupied, view, speed, Config::COIN_MAX_TRY_SEARCH_AREA_FREE,
        [&](sf::Vector2f finalPos) {
                    coin->addCoinSequence(coinType, finalPos);
                }
    );
}

// Funzione di preprazione dei parametri per il metodo generico
bool ManagerLayer::laser_handlerCallGenericUpdate(const sf::View& view, float speed, const std::vector<sf::FloatRect>& allOccupied) {
    
    LaserType laserType = laser->generateLaserType();
    Laser newLaser(laserType, {0,0}); 

    return handlerGenericUpdate(
        laser, newLaser.getLaserSize(), allOccupied, view, speed, Config::LASER_MAX_TRY_SEARCH_AREA_FREE,
        [&](sf::Vector2f finalPos) {
                    newLaser.setLaserPosition(finalPos);
                    laser->addLaser(laserType, finalPos); 
                }
    );
}

std::pair<int,bool> ManagerLayer::update(sf::Time time, const sf::View& view, const sf::FloatRect& player, float speed){
    
    coin->update(time, view);
    laser->update(time, view);
    laserBeam->update(time, view);

    // isActive dice se è "ora" di visualizzare i laser
    bool shouldDisableNormaleLaser = laserBeam->isActive();

    if(shouldDisableNormaleLaser){
        // Se ci sono ancora laser attivi configuro i laserBeam tali da aspettare per esser generati
        if(laser->areThereActiveLaser()) {
            laserBeam->waitToGen();
        } else {
            laserBeam->goToGen();
        }
    }

    // Controllo delle collisioni 
    int numberCoinHit = coin->     checkCollisions(player);
    bool hitLaser     = laser->    checkCollisions(player);
    bool hitLaserBeam = laserBeam->checkCollisions(player);
    
    // TUTTE le aree occupate da TUTTI
    std::vector<sf::FloatRect> allOccupied = getAllOccupiedAreas();
    
    if(coin_handlerCallGenericUpdate(view, speed, allOccupied)) {
        areaTaken[EntityType::TypeCoin] = coin->getOccupiedAreas();
    }

    // NON genero altri laser se [sono prossimo] / [sto genenrando] ai/i laserBeam
    if(!shouldDisableNormaleLaser) {
        if(laser_handlerCallGenericUpdate(view, speed, allOccupied)) {
            areaTaken[EntityType::TypeLaser] = laser->getOccupiedAreas();
        }
    }
    
    return std::pair<int, bool>{numberCoinHit, (hitLaser || hitLaserBeam)};
}

void ManagerLayer::draw(sf::RenderWindow& window){
    // Non faccio distinzioni tanto gestisco tutto perfettamente nella update
    coin->draw(window);
    laser->draw(window);
    laserBeam->draw(window);
}


